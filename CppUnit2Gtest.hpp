#ifndef CPPUNIT_TO_GTEST_HEADER_
#define CPPUNIT_TO_GTEST_HEADER_

#include <gtest/gtest.h>

#define CPPUNIT_NS ::CppUnit

namespace CppUnit {

    class TestCase : public testing::Test
    {
    public:
        // Take Cppunit style functions
        virtual void setUp() {}
        virtual void tearDown() {}

        // Name forward to gtest equivalents
        void SetUp() override { setUp(); }
        void TearDown() override { tearDown(); }
    };

    using TestFixture = TestCase;
// CppUnit does some odd things in the name of portability
//  We end up undoing a lot of them by default (ironically to maintain portability)
// Define to avoid the CppUnit::OStream being defined (allows for user defined custom streams)
#ifndef Cppunit2Gtest_NoDeclOStream
#   if defined(Cppunit2Gtest_StreamType)
        using OStream = Cppunit2Gtest_StreamType;
#   else
        using OStream = std::ostream;
#   endif
#   if defined(Cppunit2Gtest_CustomCout)
        inline auto stdCOut() { return Cppunit2Gtest_CustomCout; }
#   else
        inline auto& stdCOut() { return std::cout; }
#   endif
#endif

namespace to { namespace gtest {
#define CppUnit2Gtest_CHECK(condition) \
        if (!(condition)) { \
            throw std::runtime_error("Internal Check failed when running test harness " #condition ); \
        }

    template<typename FromClass>
    struct TestData {
        using TestMethodType = void(*)(FromClass&);
        TestMethodType testMethod;
        unsigned int line = 0;
        const char* testName;

        TestData(const TestMethodType testMethod_, const unsigned int line_, const char* testName_)
            : testMethod(testMethod_)
            , line(line_)
            , testName(testName_)
        {
            CppUnit2Gtest_CHECK(testMethod != nullptr);
            CppUnit2Gtest_CHECK(testName != nullptr);
            CppUnit2Gtest_CHECK(testName_ != nullptr);
        }

        TestData(const TestData&) = default;
        TestData(TestData&&) = default;
        TestData& operator=(const TestData&) = default;
        TestData& operator=(TestData&&) = default;
        // Allow casting to another class type that inherits the current one
        template<typename Derived>
        explicit TestData(TestData<Derived>& from)
            : testMethod(TestMethodType(from.testMethod))
            , line(from.line)
            , testName(from.testName)
        {
            CppUnit2Gtest_CHECK(testMethod != nullptr);
            CppUnit2Gtest_CHECK(testName != nullptr);
            CppUnit2Gtest_CHECK(from.testMethod != nullptr);
        }
    };

    template<typename TestSuite>
    struct DynamicTest : TestSuite {
        using TestSuite::TestSuite;
        using TestMethod = typename TestData<TestSuite>::TestMethodType;
        TestMethod testMethod;
        explicit DynamicTest(TestMethod testMethod_) : testMethod(testMethod_) {}
        void TestBody() override {
            auto& a = static_cast<TestSuite&>(*this);
            (testMethod)(a);
        }
    };

    template<typename TestSuite>
    int InternalRegisterTestsVector(
        std::vector<TestData<TestSuite>>& testSuiteData,
        const char* file_name,
        const int line_number,
        const char* fixtureName)
    {
        CppUnit2Gtest_CHECK(file_name != nullptr);
        CppUnit2Gtest_CHECK(fixtureName != nullptr);
        for(auto& testData : testSuiteData)
        {
            auto testMethod = testData.testMethod;
            // Register the test programmatically
            ::testing::RegisterTest(
                 fixtureName,              // name of the fixture
                 testData.testName,        // name of the test
                 nullptr, nullptr,         // argument details for parametrised tests
                 file_name, line_number,   // For the log
                 [testMethod]() -> DynamicTest<TestSuite>* { return new DynamicTest<TestSuite>(testMethod); }
             );
        }
        return __LINE__;
    }

    template<typename TestSuite>
    int InternalRegisterTests(const char* file_name, int line_number, const char* fixtureName )
    {
        std::vector<TestData<TestSuite>> tests = TestSuite::GetAllTests_();

        InternalRegisterTestsVector(tests, file_name, line_number, fixtureName);
        // return an int so we can call this statically a bit easier
        return __LINE__;
        //  (Use something that can change to avoid people doing asserts on it)
    }


}
}
}


/// Takes a suite name and creates a vector of function pointers to the functions given in the registration
#define CPPUNIT_TEST_SUITE(SuiteName) \
    using Cpp2GTest_CurrentClass = SuiteName; \
    using TestDataType = ::CppUnit::to::gtest::TestData<SuiteName>; \
    public: \
        void TestBody() override {} \
        [[nodiscard]] static auto GetAllTests_() { std::vector<TestDataType> allTestData{}

/// Takes a suite name and a base class, adds all the tests from the base class to this suite
#define CPPUNIT_TEST_SUB_SUITE(SuiteName, BaseClass) \
    using Cpp2GTest_BaseClass = BaseClass; \
    CPPUNIT_TEST_SUITE(SuiteName); \
    [&]() -> void { \
        auto base_tests = Cpp2GTest_BaseClass::GetAllTests_(); \
        for (auto& test : base_tests) {         \
            allTestData.emplace_back(test);     \
        } \
    }()

/// Adds a test to the vector of tests on the class (and allows for semicolon)
#define CPPUNIT_TEST(test_name) \
    [&](){ \
        auto test_pointer = +[](Cpp2GTest_CurrentClass& c) { \
            auto t = &Cpp2GTest_CurrentClass:: test_name ; \
            (c.*t)(); \
        }; \
        allTestData.emplace_back(test_pointer, __LINE__, #test_name); \
    }()

/// This functionality is deprecated from CppUnit, we recommend changing any usages to
///  use the more readable and expressive `ASSERT_THROW( expression, exception);`
#define CPPUNIT_TEST_EXCEPTION(test_name, exception) \
    [&](){ \
        auto test_pointer = +[](Cpp2GTest_CurrentClass& c) { \
            auto t = &Cpp2GTest_CurrentClass:: test_name ; \
            ASSERT_THROW( (c.*t)() , exception ); \
        }; \
        allTestData.emplace_back(test_pointer, __LINE__, #test_name ); \
    }()

#define CPPUNIT_TEST_FAIL(v) static_assert(false, \
    "CPPUNIT_TEST_FAIL was called with " #v ". It is not supported. \n" \
    " It's intetion is to test custom CPPUNIT macros. \n" \
    " We do not support custom macros so it is moot/pointless\n" \
    " Please rewrite the test in GTest"\
)

// We could add CPPUNIT_TEST_SUITE_ADD_CUSTOM_TESTS with some complicated types
//  If it gets requested I will have a look

// If we want CPPUNIT_TEST_SUITE_PROPERTY we have to call `::testing::Test::RecordProperty`
//  but we have to do it after SetUpTestSuite and before TearDownTestSuite
//  the macro is called between CPPUNIT_TEST_SUITE and CPPUNIT_TEST_SUITE_END (needs proof)
//   so we'd need some state on the class and set it in the `GetAllTests_` function

/// Ends the vector of tests
#define CPPUNIT_TEST_SUITE_END() return allTestData; }

/// Does the same as CPPUNIT_TEST_SUITE_END
#define CPPUNIT_TEST_SUITE_END_ABSTRACT() CPPUNIT_TEST_SUITE_END()

#define Cpp2Gtest_CONCAT(a, b) Cpp2Gtest_CONCAT_INNER(a, b)
#define Cpp2Gtest_CONCAT_INNER(a, b) a ## b
#define Cpp2Gtest_UNIQUE_NAME(base) Cpp2Gtest_CONCAT(base, __LINE__)

#define CPPUNIT_TEST_SUITE_REGISTRATION(Class_name) CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Class_name, #Class_name)

#define CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Class_name, suite_additional_name ) namespace{ \
    static const int Cpp2Gtest_UNIQUE_NAME(unused_) = \
    ::CppUnit::to::gtest::InternalRegisterTests<Class_name>(#Class_name, __LINE__, suite_additional_name); \
}

/// The following macros are for running the tests under a hierarchy,
///   we don't see the value so they all do nothing
#define CPPUNIT_REGISTRY_ADD(which, to)
#define CPPUNIT_REGISTRY_ADD_TO_DEFAULT(which)

// Wasn't tested in CppUnit, not tested here!
#if CPPUNIT_ENABLE_CU_TEST_MACROS
#   define CU_TEST_SUITE(tc)                CPPUNIT_TEST_SUITE(tc)
#   define CU_TEST_SUB_SUITE(tc,sc)         CPPUNIT_TEST_SUB_SUITE(tc,sc)
#   define CU_TEST(tm)                      CPPUNIT_TEST(tm)
#   define CU_TEST_SUITE_END()              CPPUNIT_TEST_SUITE_END()
#   define CU_TEST_SUITE_REGISTRATION(tc)   CPPUNIT_TEST_SUITE_REGISTRATION(tc)
#endif

#define CPPUNIT_TEST_FIXTURE(FixtureClass,testName) TEST_F(FixtureClass, testName)

// Asserting
#define CPPUNIT_ASSERT(condition)                                    ASSERT_TRUE(condition)
#define CPPUNIT_ASSERT_MESSAGE(message, condition)                   ASSERT_TRUE(condition) << message
#define CPPUNIT_ASSERT_EQUAL(a, b)                                   ASSERT_EQ(a, b)
#define CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, a, b)                      ASSERT_EQ(a, b) << msg
#define CPPUNIT_ASSERT_NO_THROW(expression)                          ASSERT_NO_THROW(expression)
#define CPPUNIT_ASSERT_NO_THROW_MESSAGE(msg, expression)             ASSERT_NO_THROW(expression) << message
#define CPPUNIT_ASSERT_THROW(expression, expected)                   ASSERT_THROW(expression, expected)
#define CPPUNIT_ASSERT_THROW_MESSAGE(message, expression, expected)  ASSERT_THROW(expression, expected) << message
#define CPPUNIT_ASSERT_DOUBLES_EQUAL(a,b, t)                         ASSERT_NEAR(a, b, t)
#define CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg, a, b, t)           ASSERT_NEAR(a, b, t) << msg
#define CPPUNIT_FAIL(message)                                        FAIL(message)
#define CPPUNIT_ASSERT_ASSERTION_PASS(e)                             ASSERT_NO_THROW(e)
#define CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(msg, e)                ASSERT_NO_THROW(e) << msg

// Things like assert failed are intentionally not added
//  We can add them but behaviour could be buggy
//   (I think an inline lambda counts as a function and therefor ASSERT won't end the function
//   early so we'd call the lambda, return to outer function then `ASSERT_TRUE(HasFailure());`  (?)

#endif
