#include <cppunit/extensions/HelperMacros.h>

#ifdef Cpp2Unit2Gtest_EnableMainHelperClasses

TEST(TextTestRunner, TextUiTestRunnerExists)
{
    ASSERT_NO_THROW( CppUnit::TextUi::TestRunner{} );
}

TEST(TextTestRunner, AddTest_NullTest_Throws)
{
    CppUnit::TextUi::TestRunner runner;

    ASSERT_THROW(runner.addTest(nullptr), std::invalid_argument);
}

TEST(TextTestRunner, AddRootTest)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* overallTest = registry.makeTest();

    runner.addTest(overallTest);

    ASSERT_EQ("", runner.filter) << "Adding root test should not change filter";
}

TEST(TextTestRunner, AddAllTests)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* overallTest = registry.makeTest();
    CppUnit::Test* unit = overallTest->findTest("All Tests");

    runner.addTest(unit);
    ASSERT_EQ("", runner.filter) << "Adding root test should not change filter";
}

TEST(TextTestRunner, AddNamedTest)
{
    const std::string test_name = "AddNamedTest"; 
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* overallTest = registry.makeTest();
    CppUnit::Test* unit = overallTest->findTest(test_name);
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

    CppUnit::Test* overallTest = registry.makeTest();
    runner.addTest(overallTest->findTest(test_name1));
    runner.addTest(overallTest->findTest(test_name2));
    runner.cleanFilter();
    
    ASSERT_EQ("--gtest_filter=" + test_name1 + ":" +  test_name2, runner.filter) << "Adding 2 named tests should concatenate names with colon";
}

TEST(TextTestRunner, AddDuplicateTests)
{
    const std::string test_name1 = "AddDuplicateTests";

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

    CppUnit::Test* overallTest = registry.makeTest();
    runner.addTest(overallTest->findTest(test_name1));
    runner.addTest(overallTest->findTest(test_name1));
    runner.cleanFilter();

    ASSERT_EQ("--gtest_filter=" + test_name1, runner.filter) << "Adding a named test twice should only add it once";
}

#else
TEST(MainClasses, NoMainClassesIncluded)
{
    GTEST_SKIP() << "Main helper classes are not included in this build";
}
#endif