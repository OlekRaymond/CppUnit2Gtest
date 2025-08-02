
/** Example class to show hierarchy testing.
 *
 *  Shamelessly ripped and adapted from
 *  <a href="http://c2.com/cgi/wiki?ClassHierarchyTestingInCppUnit">
 *  ClassHierarchyTestingInCppUnit</a>
 *
 */
class BoardGame {
public:
    /// expected to return true
    virtual bool reset() {return true; }
    virtual ~BoardGame() { }
};

class Chess: public BoardGame {
public:
    virtual int getNumberOfPieces() const {
        return 32;
    }
};

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/portability/Stream.h>

template<class GAMECLASS>
class BoardGameTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( BoardGameTest );
    CPPUNIT_TEST( testReset );
    // CppUnit2GTest: Removed failing test from example
    // CPPUNIT_TEST( testResetShouldFail );
    CPPUNIT_TEST_SUITE_END();
protected:
    GAMECLASS	*m_game;

public:
    // BoardGameTest()
    // {
    // }

    int countTestCases () const
    {
        return 1;
    }

    void setUp()
    {
        this->m_game = new GAMECLASS;
    }

    void tearDown()
    {
        delete this->m_game;
    }

    void testReset()
    {
        CPPUNIT_ASSERT( this->m_game->reset() );
    }

    void testResetShouldFail()
    {
        CPPUNIT_NS::stdCOut() << "The following test fails, this is intended:" << "\n";
        CPPUNIT_ASSERT( !this->m_game->reset() );
    }
};

template<class GAMECLASS>
class ChessTest : public BoardGameTest<GAMECLASS>
{
    CPPUNIT_TEST_SUB_SUITE( ChessTest, BoardGameTest<GAMECLASS> );
    CPPUNIT_TEST( testNumberOfPieces );
    CPPUNIT_TEST_SUITE_END();
public:
    // ChessTest()
    // {
    // }

    void testNumberOfPieces()
    {
        CPPUNIT_ASSERT( this->m_game->getNumberOfPieces () == 32 );
    }
};

// CppUnit2Gtest: In the original example these are registered in the main function
//  We do it here instead
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChessTest<Chess>, "Chess");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BoardGameTest<BoardGame>, "BoardGame");
