

#include <cppunit/extensions/HelperMacros.h>

namespace {
    struct Class_name : CppUnit::TestCase {
        void TestBody() override {}
        void testConstructor() {}
    };


    TEST(TestRegisteringTests, GoodValues)
    {
        ::CppUnit::to::gtest::InternalRegisterTests<Class_name>(
            // std::vector<::CppUnit::to::gtest::TestData<Class_name>>{
            //     ::CppUnit::to::gtest::TestData<Class_name>{
            //         +[](Class_name& m ){m.testConstructor();}
            //         , 0
            //         , " "
            //     }
            // },
            " ", __LINE__, " ");
        // No crash, no throw
    }

    TEST(TestRegisteringTests, NullTestName)
    {
        ASSERT_ANY_THROW(
            ::CppUnit::to::gtest::InternalRegisterTests<Class_name>(
                // std::vector<::CppUnit::to::gtest::TestData<Class_name>>{
                //     ::CppUnit::to::gtest::TestData<Class_name>{
                //         +[](Class_name& m ){m.testConstructor();}
                //         , 0
                //         , nullptr
                //     }
                // },
                " ", __LINE__, " ")
        );
        // No crash
    }

    TEST(TestRegisteringTests, NullFileName)
    {
        ASSERT_ANY_THROW(
            ::CppUnit::to::gtest::InternalRegisterTests<Class_name>(
                // std::vector<::CppUnit::to::gtest::TestData<Class_name>>{
                //     ::CppUnit::to::gtest::TestData<Class_name>{
                //         +[](Class_name& m ){m.testConstructor();}
                //         , 0
                //         , ""
                //     }
                // },
                nullptr, __LINE__, ""
            )
        );
        // No crash
    }

    TEST(TestRegisteringTests, NullFixtureName)
    {
        ASSERT_ANY_THROW(
            ::CppUnit::to::gtest::InternalRegisterTests<Class_name>(
                // std::vector<::CppUnit::to::gtest::TestData<>>{
                //     ::CppUnit::to::gtest::TestData<Class_name>{
                //         +[](Class_name& m ){m.testConstructor();}
                //         , 0
                //         , ""
                //     }
                // },
                "", __LINE__, nullptr
            )
        );
        // No crash
    }

}

