/*
 * Migration Example: From CppUnit to Google Test
 * 
 * This example shows migrating tests.
*/

struct Calculator {
    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }
    bool isEven(int n) { return n % 2 == 0; }
};

// ============================================================================
// SECTION 1: ORIGINAL CPPUNIT TESTS
// ============================================================================

#include <cppunit/extensions/HelperMacros.h>

namespace OriginalCppUnit {

class CalculatorTestCppUnit : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(CalculatorTestCppUnit);
    CPPUNIT_TEST(TestAddition);
    CPPUNIT_TEST(TestMultiplication);
    CPPUNIT_TEST(TestIsEven);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestAddition() {
        Calculator calc;
        CPPUNIT_ASSERT_EQUAL(5, calc.add(2, 3));
        CPPUNIT_ASSERT_EQUAL(-1, calc.add(2, -3));
    }

    void TestMultiplication() {
        Calculator calc;
        CPPUNIT_ASSERT_EQUAL(6, calc.multiply(2, 3));
        CPPUNIT_ASSERT_EQUAL(0, calc.multiply(0, 5));
    }

    void TestIsEven() {
        Calculator calc;
        CPPUNIT_ASSERT(calc.isEven(4));
        CPPUNIT_ASSERT(!calc.isEven(5));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(CalculatorTestCppUnit);

}

// ============================================================================
// SECTION 2: MIXED CPPUNIT AND GOOGLE TEST
// ============================================================================
// Using CppUnit2Gtest adapter - can mix both styles

#include <cppunit/extensions/HelperMacros.h>

namespace MixedStyle {

class CalculatorTestMixed : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(CalculatorTestMixed);
    CPPUNIT_TEST(TestAddition);
    CPPUNIT_TEST_SUITE_END();

public:
    // Original CppUnit tests can be kept
    void TestAddition() {
        Calculator calc;
        CPPUNIT_ASSERT_EQUAL(8, calc.add(3, 5));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( CalculatorTestMixed );

// Google Test style tests can be added to the same fixture
//  Though class used for fixtures must differ to the CppUnit one
struct CalculatorTest : CalculatorTestMixed {};

TEST_F(CalculatorTest, TestMultiplication) {
    Calculator calc;
    EXPECT_EQ(12, calc.multiply(3, 4));
    EXPECT_EQ(-6, calc.multiply(-2, 3));
}

TEST_F(CalculatorTest, TestIsEven) {
    Calculator calc;
    // Mix Google Test and CppUnit assertions
    EXPECT_TRUE(calc.isEven(10));
    CPPUNIT_ASSERT(!calc.isEven(7));
}

}

// ============================================================================
// SECTION 3: PURE GOOGLE TEST
// ============================================================================

#include <gtest/gtest.h>

namespace PureGoogleTest {

struct CalculatorTestGTest : ::testing::Test {
    // Fresh Calculator for each test
    Calculator calc;
};

TEST_F(CalculatorTestGTest, Addition) {
    EXPECT_EQ(7, calc.add(3, 4));
    EXPECT_EQ(0, calc.add(-2, 2));
}

TEST_F(CalculatorTestGTest, Multiplication) {
    EXPECT_EQ(15, calc.multiply(3, 5));
    EXPECT_EQ(0, calc.multiply(0, 100));
}

TEST_F(CalculatorTestGTest, IsEven) {
    EXPECT_TRUE(calc.isEven(2));
    EXPECT_FALSE(calc.isEven(3));
    EXPECT_TRUE(calc.isEven(0));
}

}