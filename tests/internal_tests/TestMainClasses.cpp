#include <cppunit/extensions/HelperMacros.h>

#ifdef Cpp2Unit2Gtest_EnableMainHelperClasses

namespace runners {
namespace text_runner {

TEST(TextTestRunner, TextUiTestRunnerExists)
{
    ASSERT_NO_THROW( [[maybe_unused]] CppUnit::TextUi::TestRunner a = {} );
}

TEST(TextTestRunner, AddingNullptrThrows)
{
    CppUnit::TextUi::TestRunner runner;

    ASSERT_THROW(runner.addTest(nullptr), std::invalid_argument);
}

TEST(TextTestRunner, AddRootTest)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* rootTest = registry.makeTest();

    runner.addTest(rootTest);

    ASSERT_EQ("", runner.filter) << "Adding root test should not change filter";
}

TEST(TextTestRunner, AddAllTests)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* rootTest = registry.makeTest();
    CppUnit::Test* unit = rootTest->findTest("All Tests");

    runner.addTest(unit);
    ASSERT_EQ("", runner.filter) << "Adding root test should not change filter";
}

TEST(TextTestRunner, AddNamedTest)
{
    const std::string test_name = "AddNamedTest"; 
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* rootTest = registry.makeTest();
    CppUnit::Test* unit = rootTest->findTest(test_name);
    runner.addTest(unit);
    runner.cleanFilter();

    ASSERT_EQ("--gtest_filter=" + test_name, runner.filter) << "Adding named test should set filter to that name";
}

TEST(TextTestRunner, AddMultipleNamedTests)
{
    const std::string test_name1 = "AddNamedTest";
    const std::string test_name2 = "AddMultipleNamedTests";

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* rootTest = registry.makeTest();
    runner.addTest(rootTest->findTest(test_name1));
    runner.addTest(rootTest->findTest(test_name2));
    runner.cleanFilter();
    
    ASSERT_EQ("--gtest_filter=" + test_name1 + ":" +  test_name2, runner.filter) << "Adding 2 named tests should concatenate names with colon";
}

TEST(TextTestRunner, AddDuplicateTests)
{
    const std::string test_name1 = "AddDuplicateTests";

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* rootTest = registry.makeTest();
    runner.addTest(rootTest->findTest(test_name1));
    runner.addTest(rootTest->findTest(test_name1));
    runner.cleanFilter();

    ASSERT_EQ("--gtest_filter=" + test_name1, runner.filter) << "Adding a named test twice should only add it once";
}

} // namespace runners
} // namespace text_runner

namespace registry {

TEST(Registry, GetRegistryReturnsSameInstance)
{
    CppUnit::TestFactoryRegistry& registry1 = CppUnit::TestFactoryRegistry::getRegistry();
    CppUnit::TestFactoryRegistry& registry2 = CppUnit::TestFactoryRegistry::getRegistry();

    ASSERT_EQ(&registry1, &registry2) << "getRegistry should return the same instance";
}

TEST(Registry, MakeTestReturnsRoot)
{
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* test = registry.makeTest();

    ASSERT_NE(test, nullptr) << "MakeTest should return a non-null test";
    ASSERT_EQ(test->getName(), "All Tests") << "Root test should have name 'All Tests'";
    ASSERT_NE(dynamic_cast<CppUnit::to::gtest::TestAdaptorRoot*>(test), nullptr) << "Root test should be of type TestAdaptorRoot";
}

} // namespace registry

namespace parmeterized_tests {

struct CppUnitTestAdapters : testing::TestWithParam<CppUnit::Test*> { };

struct CppUnitTestContainers : testing::TestWithParam<CppUnit::Test*> { };

TEST_P(CppUnitTestContainers, FindTestHappyPath) {
    CppUnit::Test* test = GetParam();
    ASSERT_NE(test, nullptr) << "Test parameter should not be null";

    const auto test_name = test->getChildTestAt(0)->getName();
    
    CppUnit::Test* foundTest = test->findTest(test_name);
    ASSERT_NE(foundTest, nullptr) << "Find should never return null pointer";
    ASSERT_EQ(foundTest->getName(), test_name) << "Found test should have the correct name";
}

TEST_P(CppUnitTestContainers, HasChildren) {
    CppUnit::Test* test = GetParam();
    ASSERT_NE(test, nullptr) << "Test parameter should not be null";

    const auto child_count = test->getChildTestCount();
    
    ASSERT_GT(child_count, 0) << "Test container should have child test";
    ASSERT_LT(child_count, 250) << "Test container should have reasobable amount of child tests";
    ASSERT_EQ(child_count, test->getChildTestCount()) << "Child test count should be consistent";
}

TEST_P(CppUnitTestContainers, OutOfBoundsGetChildThrows) {
    CppUnit::Test* test = GetParam();
    ASSERT_NE(test, nullptr) << "Test parameter should not be null";

    const auto child_count = test->getChildTestCount();
    ASSERT_ANY_THROW(test->getChildTestAt(-1));
    ASSERT_ANY_THROW(test->getChildTestAt(child_count));
}

TEST_P(CppUnitTestAdapters, FindTestUnhappyPath) {
    CppUnit::Test* test = GetParam();
    ASSERT_NE(test, nullptr) << "Test parameter should not be null";

    const auto test_name = "NonExistentTest_";
    
    ASSERT_THROW(test->findTest(test_name), std::invalid_argument);
}

TEST_P(CppUnitTestAdapters, HasValidName) {
    CppUnit::Test* test = GetParam();
    ASSERT_NE(test, nullptr) << "Test parameter should not be null";
    const auto name = test->getName();
    ASSERT_FALSE(name.empty()) << "Test name should not be empty";
    ASSERT_LT(name.size(), 200) << "Test name should be reasonably sized";
    if (name == "All Tests") {
        SUCCEED() << "Root test name is valid";
    } else {
        ASSERT_EQ(name.find_first_of(" \t\n\r"), std::string::npos) << "Test name: " << name << " should not contain whitespace";
    }
}

INSTANTIATE_TEST_SUITE_P(CppUnitTestContainersAdapters,
                         CppUnitTestContainers,
                         testing::Values(
                            CppUnit::TestFactoryRegistry::getRegistry().makeTest(), // Root
                            CppUnit::TestFactoryRegistry::getRegistry().makeTest()->getChildTestAt(0) // suite
                         ));
INSTANTIATE_TEST_SUITE_P(AllCppUnitTestAdapters,
                         CppUnitTestAdapters,
                         testing::Values(
                            CppUnit::TestFactoryRegistry::getRegistry().makeTest(), // Root
                            CppUnit::TestFactoryRegistry::getRegistry().makeTest()->getChildTestAt(0), // suite
                            CppUnit::TestFactoryRegistry::getRegistry().makeTest()->getChildTestAt(0)
                                ->getChildTestAt(0) // actual test
                         ));

} // namespace parmeterized_tests

namespace root_tests {

TEST(RootTest, RootReturnsRoot)
{
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* rootTest = registry.makeTest();
    
    // Assert it is root
    ASSERT_NE(rootTest, nullptr) << "MakeTest should return a non-null test";
    ASSERT_EQ(rootTest->getName(), "All Tests") << "Root test should have name 'All Tests'";
    ASSERT_NE(dynamic_cast<CppUnit::to::gtest::TestAdaptorRoot*>(rootTest), nullptr) << "Root test should be of type TestAdaptorRoot";

    CppUnit::Test* rootTest2 = rootTest->findTest("All Tests");
    ASSERT_EQ(rootTest, rootTest2) << "Finding 'All Tests' from root should return the root test itself";
}

TEST(RootTest, HasSuites)
{
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    
    CppUnit::Test* rootTest = registry.makeTest();
    const int suite_count = rootTest->getChildTestCount();
    ASSERT_GT(suite_count, 5) << "Root test should have child tests";
    auto* suite = rootTest->getChildTestAt(0);
    ASSERT_NE(suite, nullptr) << "Child tests should never be null";
    ASSERT_NE(dynamic_cast<CppUnit::to::gtest::TestAdaptorSuite*>(suite), nullptr) << "Child test should be of type TestAdaptorSuite";
}

} // namespace root_tests

namespace suite_tests {

TEST(SuiteTest, ContainsTests)
{
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    
    CppUnit::Test* rootTest = registry.makeTest();
    CppUnit::Test* suite = rootTest->getChildTestAt(0);
    ASSERT_NE(suite, nullptr) << "Child tests should never be null";
    ASSERT_NE(dynamic_cast<CppUnit::to::gtest::TestAdaptorSuite*>(suite), nullptr) << "Child test should be of type TestAdaptorSuite";

    const int test_count = suite->getChildTestCount();
    ASSERT_GT(test_count, 0) << "Suite should have child tests";

    CppUnit::Test* test = suite->getChildTestAt(0);
    ASSERT_NE(test, nullptr) << "Child tests should never be null";
    ASSERT_NE(dynamic_cast<CppUnit::to::gtest::TestAdaptorActualTest*>(test), nullptr) << "Child test should be of type TestAdaptorActualTest";
}

} // namespace suite_tests

namespace actual_test_tests {

TEST(ActualTest, NoChildren)
{
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    
    CppUnit::Test* rootTest = registry.makeTest();
    CppUnit::Test* suite = rootTest->getChildTestAt(0);
    CppUnit::Test* test = suite->getChildTestAt(0);

    ASSERT_EQ(test->getChildTestCount(), 0) << "Actual test should have no children";
    ASSERT_THROW(test->getChildTestAt(0), std::invalid_argument) << "Getting child from actual test should throw";
    ASSERT_THROW(test->findTest("Anything"), std::invalid_argument) << "Getting child from actual test should throw";
}

} // namespace actual_test_tests

#else
    TEST(MainClasses, NoMainClassesIncluded)
    {
        GTEST_SKIP() << "Main helper classes are not included in this build";
    }
#endif
