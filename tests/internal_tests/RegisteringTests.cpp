

#include <cppunit/extensions/HelperMacros.h>

namespace {
    struct Class_name : CppUnit::TestCase {
        void TestBody() override {}
        void testConstructor() {}
    };


    TEST(TestRegisteringTests, GoodValues)
    {
        std::vector<::CppUnit::to::gtest::TestData<Class_name>> all_tests {
            ::CppUnit::to::gtest::TestData<Class_name>{
                +[](Class_name& m ){ }
                , 0
                , " "
            }
        };
        ::CppUnit::to::gtest::InternalRegisterTestsVector<Class_name>(
            all_tests, " ", __LINE__, " ");
        // No crash, no throw
    }

    TEST(TestRegisteringTests, NullTestName)
    {        
        ASSERT_ANY_THROW(
            auto a = (::CppUnit::to::gtest::TestData<Class_name>{
                +[](Class_name& m ){ }
                , 0
                , nullptr
            });
        );
    }

    TEST(TestRegisteringTests, NullFileName)
    {
        std::vector<::CppUnit::to::gtest::TestData<Class_name>> all_tests {
            ::CppUnit::to::gtest::TestData<Class_name>{
                +[](Class_name& m ){ }
                , 0
                , ""
            }
        };
        ASSERT_ANY_THROW(
            ::CppUnit::to::gtest::InternalRegisterTestsVector<Class_name>(
                all_tests, nullptr, __LINE__, ""
            )
        );
        // No crash
    }

    TEST(TestRegisteringTests, NullFixtureName)
    {
        std::vector<::CppUnit::to::gtest::TestData<Class_name>> all_tests {
            ::CppUnit::to::gtest::TestData<Class_name>{
                +[](Class_name& m ){ }
                , 0
                , ""
            }
        };
        ASSERT_ANY_THROW(
            ::CppUnit::to::gtest::InternalRegisterTestsVector<Class_name>(
                all_tests, "", __LINE__, nullptr
            )
        );
        // No crash
    }

}

