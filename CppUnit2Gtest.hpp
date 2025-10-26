#ifndef CPPUNIT_TO_GTEST_HEADER_
#define CPPUNIT_TO_GTEST_HEADER_

#include <gtest/gtest.h>

#define CppUnit2Gtest
#define CppUnit2Gtest_VERSION   "0.0.0"
#define CPPUNIT_VERSION         "CppUnit2Gtest"
#define CPPUNIT_NS              ::CppUnit
#define CPPUNIT_NS_BEGIN        namespace CppUnit {
#define CPPUNIT_NS_END          }

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
//  We end up undoing them by default (ironically to maintain portability)
/// Define to avoid the CppUnit::OStream being defined (allows for user defined custom streams)
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
    // Stream operators make things forward compatable with gtest
    struct ExitingAssertion : std::stringstream { };
#define CppUnit2Gtest_CHECK(condition) \
        if (!(condition)) { \
            throw std::runtime_error("Internal Check failed when running test harness " #condition ); \
        }

    /// Holds data required for each test.
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

    /// Includes the TestBody entry point that gtest runs
    template<typename TestSuite>
    struct DynamicTest : TestSuite {
        using TestSuite::TestSuite;
        using TestMethod = typename TestData<TestSuite>::TestMethodType;
        TestMethod testMethod;
        explicit DynamicTest(TestMethod testMethod_) : testMethod(testMethod_) {}
        void TestBody() override {
            // We inherit from this so safe to cast.
            auto& a = static_cast<TestSuite&>(*this);
            try {
                (testMethod)(a);
            } catch (const ExitingAssertion& e) {
                // Hack to get around non-exiting assertions
                //  Mostly only needed when CppUnit2Gtest_AllowAssertsInConstructors is on
                FAIL() << e.str();
            }
        }
    };

    /// Registers a vector of tests from a test suite, can be less or more than overload
    template<typename TestSuite>
    size_t InternalRegisterTestsVector(
        std::vector<TestData<TestSuite>>& testSuiteData,
        const char* file_name,
        const int line_number,
        const char* fixtureName)
    {
        // Never occurs with expected usage so safe to assert.
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
                 // Any callable that returns adress of an object inheriting testing::Test 
                 [testMethod]() -> DynamicTest<TestSuite>* { return new DynamicTest<TestSuite>(testMethod); }
             );
        }
        return testSuiteData.size();
    }

    /// Register required tests from a suite 
    template<typename TestSuite>
    size_t InternalRegisterTests(const char* file_name, int line_number, const char* fixtureName )
    {
        std::vector<TestData<TestSuite>> tests = TestSuite::GetAllTests_();

        InternalRegisterTestsVector(tests, file_name, line_number, fixtureName);
        // return an int so we can call this statically a bit easier
        return tests.size();
    }

#undef CppUnit2Gtest_CHECK
}
}
}


/// Takes a suite name and creates a vector of function pointers to the functions given in the registration
#define CPPUNIT_TEST_SUITE(SuiteName) \
    using Cpp2GTest_CurrentClass = SuiteName; \
    using TestDataType = ::CppUnit::to::gtest::TestData<SuiteName>; \
    public: \
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
//  Don't add it for now

// If we want CPPUNIT_TEST_SUITE_PROPERTY we have to call `::testing::Test::RecordProperty`
//  but we have to do it after SetUpTestSuite and before TearDownTestSuite
//  the macro is called between CPPUNIT_TEST_SUITE and CPPUNIT_TEST_SUITE_END (needs proof)
//   so we'd need some state on the class and set it in the `GetAllTests_` function

// Do nothing for now
#define CPPUNIT_TEST_SUITE_PROPERTY( unused_1, unused_2 ) 

/// Ends the vector of tests
#define CPPUNIT_TEST_SUITE_END() return allTestData; } void TestBody() override {}

/// Does the same as CPPUNIT_TEST_SUITE_END but the class remains abstract
#define CPPUNIT_TEST_SUITE_END_ABSTRACT() return allTestData; }

#define Cpp2Gtest_CONCAT(a, b) Cpp2Gtest_CONCAT_INNER(a, b)
#define Cpp2Gtest_CONCAT_INNER(a, b) a ## b
#define Cpp2Gtest_UNIQUE_NAME(base) Cpp2Gtest_CONCAT(base, __LINE__)

#define CPPUNIT_TEST_SUITE_REGISTRATION(Class_name) CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Class_name, #Class_name)

#define CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Class_name, suite_additional_name ) namespace{ \
    static const size_t Cpp2Gtest_UNIQUE_NAME(unused_) = \
    ::CppUnit::to::gtest::InternalRegisterTests<Class_name>(#Class_name, __LINE__, suite_additional_name); \
}

/// The following two macros are for running the tests under a hierarchy,
///   we don't see the value so they all do nothing
#define CPPUNIT_REGISTRY_ADD(which, to)
#define CPPUNIT_REGISTRY_ADD_TO_DEFAULT(which)

// For backwards compatibility, not recommended
#if CPPUNIT_ENABLE_CU_TEST_MACROS
#   define CU_TEST_SUITE(tc)                CPPUNIT_TEST_SUITE(tc)
#   define CU_TEST_SUB_SUITE(tc,sc)         CPPUNIT_TEST_SUB_SUITE(tc,sc)
#   define CU_TEST(tm)                      CPPUNIT_TEST(tm)
#   define CU_TEST_SUITE_END()              CPPUNIT_TEST_SUITE_END()
#   define CU_TEST_SUITE_REGISTRATION(tc)   CPPUNIT_TEST_SUITE_REGISTRATION(tc)
#endif

#if defined(CppUnit2Gtest_AllowAssertsInConstructors)

/// Note: gtest args must be surrounded by brackets: 
//   CppUnit2Gtest_assertion_wrapper_(TRUE, (true))
//   CppUnit2Gtest_assertion_wrapper_(TRUE, (true) << "expected true")
#   define CppUnit2Gtest_assertion_wrapper_(gtest_assertion, args) \
    EXPECT_ ## gtest_assertion args ; if (::testing::Test::HasFailure()) throw ::CppUnit::to::gtest::ExitingAssertion{} 

#   define CppUnit2Gtest_fail_wrapper_() ADD_FAILURE()

#else // CppUnit2Gtest_AllowAssertsInConstructors
#   define CppUnit2Gtest_assertion_wrapper_(gtest_assertion, args) \
    ASSERT_ ## gtest_assertion args 

#   define CppUnit2Gtest_fail_wrapper_() FAIL()

#endif

// For backwards compatibility, not recommended
#if CPPUNIT_ENABLE_NAKED_ASSERT
#   undef assert
#   define assert(c)                        CppUnit2Gtest_assertion_wrapper_(TRUE, (c))
#   define assertEqual(e,a)                 CppUnit2Gtest_assertion_wrapper_(EQ, (e,a))
#   define assertDoublesEqual(e,a,d)        CppUnit2Gtest_assertion_wrapper_(NEAR, (e,a,d))
#   define assertLongsEqual(e,a)            CppUnit2Gtest_assertion_wrapper_(EQ, (e,a))
#endif

#define CPPUNIT_TEST_FIXTURE(FixtureClass,testName) TEST_F(FixtureClass, testName)

// Asserting
#define CPPUNIT_ASSERT(condition)                                    CppUnit2Gtest_assertion_wrapper_(TRUE, (condition))
#define CPPUNIT_ASSERT_MESSAGE(message, condition)                   CppUnit2Gtest_assertion_wrapper_(TRUE, (condition) << message)
#define CPPUNIT_ASSERT_EQUAL(a, b)                                   CppUnit2Gtest_assertion_wrapper_(EQ, (a, b))
#define CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, a, b)                      CppUnit2Gtest_assertion_wrapper_(EQ, (a, b) << msg)
#define CPPUNIT_ASSERT_NO_THROW(expression)                          CppUnit2Gtest_assertion_wrapper_(NO_THROW,(expression))
#define CPPUNIT_ASSERT_NO_THROW_MESSAGE(msg, expression)             CppUnit2Gtest_assertion_wrapper_(NO_THROW,(expression) << message)
#define CPPUNIT_ASSERT_THROW(expression, expected)                   CppUnit2Gtest_assertion_wrapper_(THROW, (expression, expected))
#define CPPUNIT_ASSERT_THROW_MESSAGE(message, expression, expected)  CppUnit2Gtest_assertion_wrapper_(THROW,(expression, expected) << message)
#define CPPUNIT_ASSERT_DOUBLES_EQUAL(a,b, t)                         CppUnit2Gtest_assertion_wrapper_(NEAR, (a, b, t))
#define CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg, a, b, t)           CppUnit2Gtest_assertion_wrapper_(NEAR, (a, b, t) << msg)
#define CPPUNIT_FAIL(message)                                        CppUnit2Gtest_fail_wrapper_() << message
#define CPPUNIT_ASSERT_ASSERTION_PASS(e)                             CppUnit2Gtest_assertion_wrapper_(NO_THROW, (e))
#define CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(msg, e)                CppUnit2Gtest_assertion_wrapper_(NO_THROW, (e) << msg)
#define CPPUNIT_ASSERT_LESS(expected, actual)                        CppUnit2Gtest_assertion_wrapper_(LT, (actual, expected))
#define CPPUNIT_ASSERT_LESSEQUAL(expected, actual)                   CppUnit2Gtest_assertion_wrapper_(LE, (actual, expected))
#define CPPUNIT_ASSERT_GREATER(expected, actual)                     CppUnit2Gtest_assertion_wrapper_(GT, (actual, expected))
#define CPPUNIT_ASSERT_GREATEREQUAL(expected, actual)                CppUnit2Gtest_assertion_wrapper_(GE, (actual, expected))

// These aren't in CppUnit but we can be nicer to the user
#define CPPUNIT_ASSERT_LESS_MESSAGE(msg, expected, actual)           CppUnit2Gtest_assertion_wrapper_(LT, (actual, expected) << msg)
#define CPPUNIT_ASSERT_GREATER_MESSAGE(msg, expected, actual)        CppUnit2Gtest_assertion_wrapper_(GT, (actual, expected) << msg)
#define CPPUNIT_ASSERT_LESSEQUAL_MESSAGE(msg, expected, actual)      CppUnit2Gtest_assertion_wrapper_(LE, (actual, expected) << msg)
#define CPPUNIT_ASSERT_GREATEREQUAL_MESSAGE(msg, expected, actual)   CppUnit2Gtest_assertion_wrapper_(GE, (actual, expected) << msg)

#define CppUnit2Gtest_FailCompilation_NotSupported_ static_assert(false, \
    "This CppUnit macro is not supported. Please rewrite this test in GTest or with implmented macros")

// Some macros are intentionally not added
//  Most are to allow users to define them themselves if they wish
//      CPPUNIT_TEST_SUITE_PROPERTY
//  Some however should really be re-written
// These include:
#define CPPUNIT_TEST_SUITE_ADD_TEST(t)              CppUnit2Gtest_FailCompilation_NotSupported_
#define CPPUNIT_ASSERT_ASSERTION_FAIL(t)            CppUnit2Gtest_FailCompilation_NotSupported_
#define CPPUNIT_EXTRACT_EXCEPTION_TYPE_(t)          CppUnit2Gtest_FailCompilation_NotSupported_
#define CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE(t)    CppUnit2Gtest_FailCompilation_NotSupported_
#define CppUnit2Gtest_BadCast_ static_assert(false, "This did a C style cast and is unlikely to be what you wanted")
#define CPPUNIT_STATIC_CAST(a, b)   CppUnit2Gtest_BadCast_
#define CPPUNIT_CONST_CAST(a,b)     CppUnit2Gtest_BadCast_

// Macros that (now) do nothing have been implemented as such
//  These were for exporting plugins (not adding a main).
#define CPPUNIT_PLUGIN_IMPLEMENT_MAIN()
#define CPPUNIT_PLUGIN_IMPLEMENT()
#define CPPUNIT_WRAP_COLUMN 1

#if defined(Cpp2Unit2Gtest_EnableMainHelperClasses)

namespace CppUnit {

struct Test {
    virtual int getChildTestCount() const = 0;
    virtual std::string getName() const = 0;
    // constness removed,
    //  returning editable result from a const function seems wrong
    //  TODO: Review impact and consider refactor to singleton lookup
    virtual Test* getChildTestAt(int index) /*const*/ = 0;
    virtual Test* findTest(const std::string& testName) /*const*/ = 0;
};

namespace to { namespace gtest {
    inline size_t ToSize_t(int value) {
        if (value < 0) { throw std::runtime_error("Negative value cannot be converted to size_t"); }
        return static_cast<size_t>(value);
    }
    // This is the actual test data (never children)
    struct TestAdaptorActualTest : Test
    {
        const testing::TestInfo* testInfo;
        
        explicit TestAdaptorActualTest(const testing::TestInfo* testInfo_) : testInfo(testInfo_) {}
        
        int getChildTestCount() const override { return 0; }

        Test* getChildTestAt([[maybe_unused]] int index) override { throw std::invalid_argument("No child tests available"); }
        Test* findTest(const std::string& testName) override {
            return (testInfo->name() == testName) ? this : throw std::invalid_argument("Test not found: " + testName);
        }

        std::string getName() const override { return testInfo->name(); }
    };
    
    inline std::vector<TestAdaptorActualTest> CreateTests(const testing::TestSuite* testSuite) {
        std::vector<TestAdaptorActualTest> tests;
        const auto test_count = testSuite->total_test_count();
        tests.reserve(ToSize_t(test_count));
        for (int i = 0; i < test_count; ++i) {
            const testing::TestInfo* test_info = testSuite->GetTestInfo(i);
            tests.emplace_back(test_info);
        }
        return tests;
    }

    struct TestAdaptorSuite : public Test {
        const testing::TestSuite* testSuite;
        std::vector<TestAdaptorActualTest> tests = CreateTests(testSuite);
        
        explicit TestAdaptorSuite(const testing::TestSuite* testSuite_)
            : testSuite(testSuite_), tests(CreateTests(testSuite_)) {}
        
        int getChildTestCount() const override {
            const size_t test_count = testSuite->total_test_count();
            if (test_count > static_cast<size_t>(std::numeric_limits<int>::max())) {
                throw std::runtime_error("Number of tests exceeds maximum int value");
            }
            return static_cast<int>(test_count);
        }
        const Test* getChildTestAt(int index) const { return &tests.at(ToSize_t(index)); }
        Test* getChildTestAt(int index) override    { return &tests.at(ToSize_t(index)); }
        std::string getName() const override { return testSuite->name(); }
        Test* findTest(const std::string& testName) override {
            for (auto& test : tests) {
                if (test.getName() == testName) {
                    return &test;
                }
            }
            throw std::invalid_argument("Test not found: " + testName);
        }
    };

    inline std::vector<TestAdaptorSuite> CreateSuites() {
        std::vector<TestAdaptorSuite> suites;
        const testing::UnitTest* unit_test = testing::UnitTest::GetInstance();
        // CppUnit uses int as size type so we shall too (bad)
        const int test_suite_count = unit_test->total_test_suite_count();
        suites.reserve(ToSize_t(test_suite_count));
        for (int i = 0; i < test_suite_count; ++i) {
            const testing::TestSuite* test_suite = unit_test->GetTestSuite(i);
            suites.emplace_back(test_suite);
        }
        return suites;
    }

    struct TestAdaptorRoot : public Test {
        std::vector<TestAdaptorSuite> suites = CreateSuites();

        // Returns number of test suites
        int getChildTestCount() const override {
            const size_t test_count = suites.size();
            if (test_count > static_cast<size_t>(std::numeric_limits<int>::max())) {
                throw std::runtime_error("Number of tests exceeds maximum int value");
            }
            return static_cast<int>(test_count);
        }

        Test* getChildTestAt(int index) override { return &suites.at(ToSize_t(index)); }

        std::string getName() const override { return "All Tests"; }

        Test* findTest(const std::string& testName) override {
            if (testName == "All Tests") {
                return this;
            }
            for (auto& suite : suites) {
                if (suite.getName() == testName) {
                    return &suite;
                }
                
                try { // Follow CppUnit's decision
                    return suite.findTest(testName);
                } catch (const std::invalid_argument&) { }
                
            }
            throw std::invalid_argument("Test not found: " + testName);
        }
    };

}} // namespace to::gtest

struct TestFactoryRegistry {
    TestFactoryRegistry() = default;
    
    static TestFactoryRegistry& getRegistry ([[maybe_unused]] const std::string& name="All Tests") {
        // return singleton registry
        static TestFactoryRegistry registry{};
        return registry;
    }

    Test* makeTest() {
        static to::gtest::TestAdaptorRoot root;
        return &root;
    }
    // Required by
    //  printTestNames(registry.makeTest(), Indentation(0));
};

struct TextTestRunner {
    // Required by
    //  CPPUNIT_NS::TextUi::TestRunner runner;
    std::string filter = "";

    // Not a cppunit method
    void addTest(const std::string& testName) {
        if (testName.empty()) {
            throw std::invalid_argument("Test name cannot be empty");
        }
        // if test (a suite/selection) is root, do nothing
        if (testName == "All Tests") {  return; }
        // check if it's already there
        if (!filter.empty()) {
            const auto position = filter.find(testName + ":");
            const auto isNPos = std::string::npos == position;
            if (position == 0) { return; }
            if (!isNPos && filter.size() > position && filter[position-1]==':') { return; }
        }
        // else filter everything else out then add this one
        filter += testName + ":";
    }

    void addTest(Test* test) {
        if (test == nullptr) {
            throw std::invalid_argument("Test cannot be null");
        }
        const auto test_name = test->getName(); 
        addTest(test_name);
    }
    // Required by
    //  runner.addTest(registry.makeTest());
    //  runner.addTest(overallTest->findTest(unit));

    // TODO: Should be protected then refactor tests
    void cleanFilter() {
        if (filter.empty()) { return; }
        filter = filter.erase(filter.size() - 1, 1); // Remove trailing colon
        filter = "--gtest_filter=" + filter;
    }

    // TODO: Mock and test:
    //  testing::InitGoogleTest
    //  RUN_ALL_TESTS
    bool run(
            [[maybe_unused]] const std::string& testPath="",
            [[maybe_unused]] bool doWait=false,
            [[maybe_unused]] bool doPrintResult=true,
            [[maybe_unused]] bool doPrintProgress=true
        ) {
        int argc = filter.empty() ? 1 : 2;
        cleanFilter();
        std::string fake_exe_name = "executable_name";
        char* argv_data[] = { fake_exe_name.data(), filter.data() };
        testing::InitGoogleTest(&argc, argv_data);
        return 0 == RUN_ALL_TESTS();
    }
    // Required by
    // 	const auto ok = runner.run(string(), false);
    //  ok:
    //	  std::cerr << (ok ? "Tests successful\n" : "Tests failed\n");
    // 	  return !ok;
    
};
namespace TextUi { 
    using TestRunner = ::CppUnit::TextTestRunner; 
    // Class is deprecated: https://cppunit.sourceforge.net/doc/cvs/namespace_text_ui.html
    // Does not have same interface as: https://cppunit.sourceforge.net/doc/cvs/class_test_runner.html
    // Has similar (enough) interface as: https://cppunit.sourceforge.net/doc/cvs/class_text_test_runner.html
}

} // namespace CppUnit

#endif // Cpp2Unit2Gtest_EnableMainHelperClasses

#endif // C++11 style header guard CPPUNIT_TO_GTEST_HEADER_
