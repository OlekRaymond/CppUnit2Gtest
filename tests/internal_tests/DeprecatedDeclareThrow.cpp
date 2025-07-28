/// Tests the deprecated ability to declare a test that should fail with an exception
///  We do not recommend using this and any test using it should be refactored:
///   to use `ASSERT_THROWS(expression, exception)`

#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

class ThrowingTests : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( ThrowingTests );
    CPPUNIT_TEST_EXCEPTION( deprecated, std::logic_error );
    CPPUNIT_TEST( prefered );
    CPPUNIT_TEST_SUITE_END();
protected:
    void deprecated() {
        throw std::logic_error("some logic_error");
    }
    void prefered() {
        CPPUNIT_ASSERT_THROW ( throw std::logic_error("some logic_error"), std::logic_error );
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( ThrowingTests );

