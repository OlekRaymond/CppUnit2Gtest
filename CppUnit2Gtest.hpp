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
// Define to avoid the CppUnit::OStream being defined (allows for user defined custom streams)
#ifndef Cppunit2Gtest_NoDeclOStream
#    if defined(Cppunit2Gtest_StreamType)
    using OStream = Cppunit2Gtest_StreamType;
#    else
    using OStream = std::ostream;
#    endif
#endif

namespace to { namespace gtest {
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
        {}

        TestData(const TestData&) = default;
        TestData(TestData&&) = default;
        TestData& operator=(const TestData&) = default;
        TestData& operator=(TestData&&) = default;
    };

    template<typename TestSuite>
    struct DynamicTest : TestSuite {
        using TestSuite::TestSuite;
        using TestMethod = typename TestData<TestSuite>::TestMethodType;
        TestMethod testMethod;
        DynamicTest(TestMethod testMethod_) : testMethod(testMethod_) {}
        void TestBody() override {
            auto& a = static_cast<TestSuite&>(*this);
            (testMethod)(a);
        }
    };

    template<typename Class>
    int InternalRegisterTests(std::vector<TestData<Class>>&& tests, const char* file_name, int line_number, const char* fixtureName )
    {
        for(auto& testData : tests)
        {
            auto& testMethod = testData.testMethod;
            // Register the test programmatically
            ::testing::RegisterTest(
                fixtureName,                        // name of the fixture
                testData.testName,                  // name of the test
                nullptr, nullptr,         // argument details for parametrised tests
                file_name, line_number,             // For the log
                [testMethod]() -> DynamicTest<Class>* { return new DynamicTest<Class>(testMethod); } );
        }
        // return an int so we can call this statically a bit easier
        return __LINE__;
        //  (Use something that can change to avoid people doing asserts on it)
    }


}
}
    // Close namespaces

}


/// Takes a suite name and creates a vector of function pointers to the functions given in the registration
#define CPPUNIT_TEST_SUITE(SuiteName) \
    using Cpp2GTest_CurrentClass = SuiteName; \
    using TestDataType = ::CppUnit::to::gtest::TestData<SuiteName>; \
    public: auto GetAllTests_() { std::vector<TestDataType> allTestData{}

/// Adds a test to the vector of tests on the class (and allows for semicolon)
#define CPPUNIT_TEST(test_name) [&](){ \
    const auto test_pointer = +[](Cpp2GTest_CurrentClass& c) { c. test_name (); }; \
    allTestData.emplace_back(test_pointer, __LINE__, #test_name); }()

/// Ends the vector of tests
#define CPPUNIT_TEST_SUITE_END() } }

#define CPPUNIT_TEST_SUITE_REGISTRATION(Class_name) CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Class_name, #Class_name)

#define CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Class_name, suite_additional_name ) namespace{ \
static const int unused = ::CppUnit::to::gtest::InternalRegisterTests(Class_name{}.GetAllTests_(), #Class_name, __LINE__, suite_additional_name); }
// TODO:         ^^^^^^ requires a unique name


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

#endif
