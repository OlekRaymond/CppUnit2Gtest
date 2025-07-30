#include <cppunit/extensions/HelperMacros.h>

namespace {

    struct ExampleTestSuite : CppUnit::TestCase {
        void testConstructor() {}
    };

    TEST(TestTestData, GoodValues) {
        ::CppUnit::to::gtest::TestData<ExampleTestSuite> test_data{
            +[](ExampleTestSuite& e){ e.testConstructor(); }, 0, ""
        };
        ASSERT_TRUE(test_data.line == 0);
        // Did not throw
    }

    TEST(TestTestData, NullName) {
        ASSERT_ANY_THROW(

            auto c = (::CppUnit::to::gtest::TestData<ExampleTestSuite>{
                +[](ExampleTestSuite& e){ e.testConstructor(); }, 0, nullptr
            } );

        );
    }

}