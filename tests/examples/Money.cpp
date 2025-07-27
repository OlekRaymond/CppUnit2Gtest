

#include <string>
#include <stdexcept>
#include <cppunit/portability/Stream.h>    // or <iostream> if portability is not an issue

struct IncompatibleMoneyError : std::runtime_error
{
    IncompatibleMoneyError() : std::runtime_error( "Incompatible moneys" ) { }
};

class Money
{
public:
    Money( double amount, std::string currency )
      : m_amount( amount )
      , m_currency( currency )
    { }

    double getAmount() const { return m_amount; }

    std::string getCurrency() const { return m_currency; }

    bool operator ==( const Money& other ) const { return m_amount == other.m_amount  && m_currency == other.m_currency; }

    bool operator !=( const Money &other ) const { return !(*this == other); }

    Money &operator +=( const Money &other )
    {
        if ( m_currency != other.m_currency ) {
            throw IncompatibleMoneyError();
        }

        m_amount += other.m_amount;
        return *this;
    }

private:
    double m_amount;
    std::string m_currency;
};


// CppUnit2Gtest Note: the following comment no longer applies

// The function below could be prototyped as:
// inline std::ostream &operator <<( std::ostream &os, const Money &value )
// If you know that you will never compile on a platform without std::ostream
// (such as embedded vc++ 4.0; though even that platform you can use STLPort)
inline CPPUNIT_NS::OStream &operator <<( CPPUNIT_NS::OStream &os, const Money &value )
{
    return os << "Money< value =" << value.getAmount() << "; currency = " << value.getCurrency() << ">";
}

#include <cppunit/extensions/HelperMacros.h>

class MoneyTest : public CPPUNIT_NS::TestFixture
{
    using Cpp2GTest_CurrentClass = MoneyTest;
    using TestDataType = ::CppUnit::to::gtest::TestData<MoneyTest>;

public:
    void TestBody() override {}
    [[nodiscard]] auto GetAllTests_()  {
        std::vector<TestDataType> allTestData{};
        [&]() {
            const auto test_pointer = +[](Cpp2GTest_CurrentClass& c) { auto t = Cpp2GTest_CurrentClass::testConstructor; (c.*t)(); };
            allTestData.emplace_back(test_pointer, 65, "testConstructor");
        }();

        return allTestData;
    }
public:
    void setUp()    {};
    void tearDown() {};

    void testConstructor() {
        // Set up
        const std::string currencyFF( "FF" );
        const double longNumber = 1234.5678;

        // Process
        Money money( longNumber, currencyFF );

        // Check
        CPPUNIT_ASSERT_DOUBLES_EQUAL( longNumber, money.getAmount(), 0.0001 );
        CPPUNIT_ASSERT_EQUAL( currencyFF, money.getCurrency() );
    }

    void testEqual() {
        // Set up
        const Money money123FF( 123, "FF" );
        const Money money123USD( 123, "USD" );
        const Money money12FF( 12, "FF" );
        const Money money12USD( 12, "USD" );

        // Process & Check
        CPPUNIT_ASSERT( money123FF == money123FF );    // ==
        CPPUNIT_ASSERT( money12FF != money123FF );     // != amount
        CPPUNIT_ASSERT( money123USD != money123FF );   // != currency
        CPPUNIT_ASSERT( money12USD != money123FF );    // != currency and != amount
    }

    void testAdd() {
        // Set up
        const Money money12FF( 12, "FF" );
        const Money expectedMoney( 135, "FF" );

        // Process
        Money money( 123, "FF" );
        money += money12FF;

        // Check
        CPPUNIT_ASSERT_EQUAL( expectedMoney, money );           // add works
        CPPUNIT_ASSERT( &money == &(money += money12FF) );  // add returns ref. on 'this'.
    }

    void testAddThrow() {
        // Set up
        const Money money123FF( 123, "FF" );

        // Process
        Money money( 123, "USD" );
        CPPUNIT_ASSERT_THROW( money += money123FF, IncompatibleMoneyError );
    }
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( MoneyTest );
