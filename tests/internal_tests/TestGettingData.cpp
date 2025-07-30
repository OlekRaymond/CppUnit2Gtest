
#include <cppunit/extensions/HelperMacros.h>

#include <utility>

namespace {

    struct S : CPPUNIT_NS::TestFixture {
        CPPUNIT_TEST_SUITE(S);
        CPPUNIT_TEST(help);
        CPPUNIT_TEST_SUITE_END();
        void help() {}
    };

    TEST(TestGettingData, Good) {
        auto all = S{}.GetAllTests_();
        ASSERT_EQ(all.size(), 1);
        ASSERT_EQ(all.front().testName, std::string{"help"});
        ASSERT_NE(all.front().testMethod, nullptr);
        ASSERT_NE(all.front().testName, nullptr);
    }

    class Monkey
    {
    public:
        Monkey(const double amount, std::string  currency )
          : m_amount( amount )
          , m_currency(std::move( currency ))
        { }

        [[nodiscard]] double getAmount() const { return m_amount; }

        [[nodiscard]] std::string getCurrency() const { return m_currency; }

    private:
        double m_amount;
        std::string m_currency;
    };

    class MonkeyTest : public CPPUNIT_NS::TestFixture
    {
    public:
        CPPUNIT_TEST_SUITE(MonkeyTest);
        CPPUNIT_TEST(testConstructor);
        CPPUNIT_TEST_SUITE_END();

        void testConstructor() const {
            // Set up
            const std::string currencyFF( "FF" );
            constexpr double longNumber = 1234.5678;

            // Process
            Monkey money( longNumber, currencyFF );

            // Check
            CPPUNIT_ASSERT_DOUBLES_EQUAL( longNumber, money.getAmount(), 0.0001 );
            CPPUNIT_ASSERT_EQUAL( currencyFF, money.getCurrency() );
        }

    };

    TEST(TetsGettingData, MocksMoney) {
        auto all = MonkeyTest {}.GetAllTests_();
        ASSERT_EQ(all.size(), 1);
        ASSERT_EQ(all.front().testName, std::string{"testConstructor"});
        ASSERT_NE(all.front().testMethod, nullptr);
        ASSERT_NE(all.front().testName, nullptr);
    }

    TEST(TetsGettingData, RegisterMonkey) {
        auto all = MonkeyTest {}.GetAllTests_();
        // ::CppUnit::to::gtest::InternalRegisterTests(std::move(all), "", 0, "");
    }
}

