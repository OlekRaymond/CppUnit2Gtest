#include <cppunit/extensions/HelperMacros.h>

namespace {

    struct ExampleTestSuite : CppUnit::TestCase {
        void TestBody() {}
        void testConstructor() { called = true; }
        bool called = false;
    };

    TEST(TestTestData, GoodValues) {
        ::CppUnit::to::gtest::TestData<ExampleTestSuite> test_data{
            +[](ExampleTestSuite& e){ e.testConstructor(); }, 0, ""
        };
        ASSERT_TRUE(test_data.line == 0);
        ASSERT_TRUE(test_data.testMethod != nullptr);
        ASSERT_TRUE(test_data.testName != nullptr );
        ASSERT_TRUE(test_data.testName == std::string{""});

        ExampleTestSuite someTestSuite{};
        (*test_data.testMethod)(someTestSuite);
        ASSERT_TRUE(someTestSuite.called);
    }

    TEST(TestTestData, NullName) {
        ASSERT_ANY_THROW(

            [[maybe_unused]] auto c = (::CppUnit::to::gtest::TestData<ExampleTestSuite>{
                +[](ExampleTestSuite& e){ e.testConstructor(); }, 0, nullptr
            } );

        );
    }
}
