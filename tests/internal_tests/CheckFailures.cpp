/// Checks that asserts can fail
///  Based of the original Simple.cpp example from CppUnit

#include <cppunit/extensions/HelperMacros.h>

#include "gtest/gtest-spi.h"

/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class CheckAssertionFailures : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( CheckAssertionFailures );
  CPPUNIT_TEST( example );
  CPPUNIT_TEST( anotherExample );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testEquals );
  CPPUNIT_TEST_SUITE_END();

protected:
  double m_value1 = -1.0;
  double m_value2 = -2.0;

public:
    void setUp() final {
        m_value1 = 2.0;
        m_value2 = 3.0;
    }

protected:
    void example() const {
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.1, 0.5 );
      ASSERT_NEAR(1.0, 1.1, 0.5);
      // This doesn't work (?)
      // EXPECT_FATAL_FAILURE( ( CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, 1.1, 0.05 ); ), "");
      //  so recreate it:
      EXPECT_FATAL_FAILURE( ASSERT_NEAR(1.0, 1.1, 0.05); , "");
      EXPECT_FATAL_FAILURE(CPPUNIT_ASSERT( 1 == 0 ), "1 == 0");
      CPPUNIT_ASSERT( 1 == 1 );
    }

    void anotherExample() const {
        EXPECT_FATAL_FAILURE(CPPUNIT_ASSERT (1 == 2), "1 == 2");
        CPPUNIT_ASSERT(!(1 == 2));
        ASSERT_NE(1, 2);
    }
    void testAdd() const {
      double result = m_value1 + m_value2;
      EXPECT_FATAL_FAILURE(CPPUNIT_ASSERT( (0.2 + 0.3) == 6.0 ), "Actual: false");
      EXPECT_FATAL_FAILURE(CPPUNIT_ASSERT_EQUAL( (0.2 + 0.3), 6.0 ), "6.0");
      ASSERT_DOUBLE_EQ( result, 5.0 );
      ASSERT_NE( result, 6.0 );
    }
    void testEquals() const {
      long* l1 = new long(12);
      long* l2 = new long(12);

      CPPUNIT_ASSERT_EQUAL( 12, 12 );
      CPPUNIT_ASSERT_EQUAL( 12L, 12L );
      CPPUNIT_ASSERT_EQUAL( *l1, *l2 );
      ASSERT_NE( l1, l2 );

      delete l1;
      delete l2;

      CPPUNIT_ASSERT( 12L == 12L );
      EXPECT_FATAL_FAILURE(CPPUNIT_ASSERT_EQUAL( 12, 13 ), "12");
      ASSERT_NE( 12, 13 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.0, 11.99, 0.5 );
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( CheckAssertionFailures );

