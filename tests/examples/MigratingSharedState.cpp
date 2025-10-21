/*
 * Incremental Migration Example: From CppUnit to Google Test
 * 
 * This single file demonstrates how you can gradually migrate from CppUnit
 * to Google Test using CppUnit2Gtest as a bridge. All sections compile and
 * run together, showing different migration stages.
*/
 
#include <string_view>

static int expensive_operatations = 0;

struct /*Imaginary*/ Database final {
	Database() {
		// Assume creating a connection to the database is expensive
		++expensive_operatations;
		// Though in this situation I'd have a single instance
		//  that lives for the entire program
	}
	Database(Database&&) = delete;
	Database(const Database&) = delete;
	auto& operator=(Database&&) = delete;
	auto& operator=(const Database&) = delete;
	struct Transaction final {
		Transaction() { /*Starts a transaction*/ }
		void execute([[maybe_unused]] std::string_view sql_statement) { /*Does Something*/ }
		void commit() {}
		~Transaction() { /*Aborts the transaction*/ }
	};
	Transaction create_transaction() { return Transaction{}; }
	void DeleteData() { }
};

// ============================================================================
// SECTION 1: ORIGINAL CPPUNIT TESTS
// ============================================================================

#include <cppunit/extensions/HelperMacros.h>

namespace WrittenAsIfCppUnitCode {

class DatabaseTestCppUnit : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE( DatabaseTestCppUnit );
    CPPUNIT_TEST( TestCommand1 );
    CPPUNIT_TEST( TestCommand2 );
    CPPUNIT_TEST_SUITE_END();
public:
    Database database;
	// In both CppUnit and Gtest constructors are called once per test 
	DatabaseTestCppUnit() : database{} { }	
	
    // Set up and tear down methods are also called before and after each test
    void tearDown() override {
        // tearDown method is the same as CppUnit
        //  (runs after every test)
        database.DeleteData();
    }

	void TestCommand1() {
        auto transaction = database.create_transaction();
		transaction.execute("Robert'); DROP TABLE Students;");
		transaction.commit();
    }

    void TestCommand2() {
        auto transaction = database.create_transaction();
        transaction.execute(
            "INSERT INTO students (first_name, middle_name, last_name) VALUES ('Bobby', '', 'Tables');"
        );
        transaction.commit();
    }

    // For demonstation purposes only:
    static void TearDownTestSuite() {
        ASSERT_EQ(expensive_operatations, 2); // one for each test.
        std::cout << "DatabaseTestCppUnit completed 2 expensive operations\n"
            << "  (constructor is called once for each test)\n";
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( DatabaseTestCppUnit );

}

// ============================================================================
// SECTION 2: CPPUNIT WITH CPPUNIT2GTEST ADAPTER
// ============================================================================
//  Some code changes are required for tests that share state
//  Tests can be written as gtest or CppUnit ones

#include <cppunit/extensions/HelperMacros.h>
#include <memory>

namespace CppUnit2Gtest {

class DatabaseTestCppUnit2Gtest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE( DatabaseTestCppUnit2Gtest );
    CPPUNIT_TEST( TestCommand1 );
    CPPUNIT_TEST( TestCommand2 );
    CPPUNIT_TEST_SUITE_END();
 public:
    static Database* database;

    static void SetUpTestSuite() {
        // Call the google test function that runs once 
        //  before the suite instead of doing it every time
        expensive_operatations = 0;
        database = new Database{};
        // Note it should not interact with members in this class
    }
	
    static void TearDownTestSuite() {
        delete database;
        ASSERT_EQ(expensive_operatations, 1); // one for the entire suite
        std::cout << "DatabaseTestCppUnit2Gtest completed 1 expensive operation\n"
                  << "  (constructor is called once for the entire suite)\n";
    }

    void tearDown() override { DatabaseTestCppUnit2Gtest::database->DeleteData(); }

    // Tests can be left how they were in CppUnit
	void TestCommand1() {
        auto transaction = database->create_transaction();
		transaction.execute("Robert'); DROP TABLE Students;");
		transaction.commit();
    }

    void TestCommand2() {
        auto transaction = database->create_transaction();
        transaction.execute(
            "INSERT INTO students (first_name, middle_name, last_name) VALUES ('Bobby', '', 'Tables');"
        );
        transaction.commit();
    }
};
Database* DatabaseTestCppUnit2Gtest::database = nullptr;

// CppUnit tests still need to be registered
CPPUNIT_TEST_SUITE_REGISTRATION( DatabaseTestCppUnit2Gtest );

// GTest's macros can still be used
//  Though class used for fixtures must differ to the CppUnit one
struct DatabaseTestCppUnit2GtestFixture : DatabaseTestCppUnit2Gtest {};

TEST_F(DatabaseTestCppUnit2GtestFixture, TestCommand1) {
    auto transaction = database->create_transaction();
    transaction.execute("Robert'); DROP TABLE Students;");
    transaction.commit();
    // Even the assertion macros
    ASSERT_TRUE(true);
    // Or keep using CppUnit's
    CPPUNIT_ASSERT(true);
}

TEST_F(DatabaseTestCppUnit2GtestFixture, TestCommand2) {
    auto transaction = database->create_transaction();
    transaction.execute(
        "INSERT INTO students (first_name, middle_name, last_name) VALUES ('Bobby', '', 'Tables');"
    );
    transaction.commit();
}

}
