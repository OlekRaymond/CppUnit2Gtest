/// CppUnit2Gtest:
///  The following tests failed in commited example
///  We have made them pass so we can use this in our testing
///  To see failing tests please look through the internal tests

#include <cppunit/extensions/HelperMacros.h>

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class ExampleTestCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( ExampleTestCase );
    CPPUNIT_TEST( example );
    CPPUNIT_TEST( anotherExample );
    CPPUNIT_TEST( testAdd );
    CPPUNIT_TEST( testEquals );
    CPPUNIT_TEST_SUITE_END();

protected:
    double m_value1;
    double m_value2;

public:
    void setUp() {
        m_value1 = 2.0;
        m_value2 = 3.0;
    }

protected:
    void example() {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.1, 0.5 );
        CPPUNIT_ASSERT( 1 == 1 );
    }
    void anotherExample() {
        CPPUNIT_ASSERT (1 == 1);
    }

    void testAdd() {
        double result = m_value1 + m_value2;
        CPPUNIT_ASSERT( result == 5.0 );
    }

    void testEquals()
    {
        // CppUnit2GTest:
        //   This test leaks memory if the test fails
        //    please never do this
        long* l1 = new long(12);
        long* l2 = new long(12);

        CPPUNIT_ASSERT_EQUAL( 12, 12 );
        CPPUNIT_ASSERT_EQUAL( 12L, 12L );
        CPPUNIT_ASSERT_EQUAL( *l1, *l2 );

        delete l1;
        delete l2;

        CPPUNIT_ASSERT( 12L == 12L );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.0, 11.99, 0.5 );
    }
};

#include <cppunit/config/SourcePrefix.h>

CPPUNIT_TEST_SUITE_REGISTRATION( ExampleTestCase );

class FixtureTest : public CPPUNIT_NS::TestFixture
{
};

CPPUNIT_TEST_FIXTURE(FixtureTest, testEquals)
{
    CPPUNIT_ASSERT_EQUAL( 12, 12 );
}

CPPUNIT_TEST_FIXTURE(FixtureTest, testAdd)
{
    double result = 2.0 + 2.0;
    CPPUNIT_ASSERT( result == 4.0 );
}
