# Examples

This folder contains examples how to use CppUnit2Gtest. Most examples are taken from CppUnit's repo and refactored into a single file. These are: [Hierarchy.cpp](Hierarchy.cpp), [Money.cpp](Money.cpp) and [Simple.cpp](Simple.cpp). 

The migration examples demonstrate how to incrementally transition from CppUnit to Google Test using CppUnit2Gtest. Each example is self-contained in a single file with all tests passing.

## Migration Philosophy

All migration approaches are designed to be **incremental**, allowing you to maintain **mixed codebases** with both Google Test and CppUnit-style tests running side by side. This means you can migrate your test suite gradually without breaking existing functionality.

## Migration Examples

Two comprehensive migration examples are provided, each one contains different stages of the migration process.

### 1. [Migration.cpp](Migration.cpp)
Shows the migration pattern for tests that don't share state between test cases:
- **Stage 1**: Original CppUnit tests
- **Stage 2**: Mixed CppUnit/Google Test using the adapter
- **Stage 3**: Pure Google Test implementation

### 2. [MigratingSharedState.cpp](MigratingSharedState.cpp)
Unfortunately, this example is needed because tests that share expensive resources require more complex handling due to fundamental differences in how CppUnit and Google Test manage test lifecycle:
- Database connections, file handles, or other costly setup operations
- Demonstrates proper use of `SetUpTestSuite()` and `TearDownTestSuite()`
- Shows how to handle the fundamental difference in constructor behavior

**Best for**: Integration tests or tests with expensive setup/teardown operations.

## Common Migration Challenges

### Constructor Behavior Change
**CppUnit**: Test class constructors run once per test suite  
**Google Test**: Test class constructors run once per individual test

**Solution**: Move shared setup logic from constructors to Google Test's static lifecycle methods:
- `SetUpTestSuite()` - runs once before all tests in the suite
- `TearDownTestSuite()` - runs once after all tests in the suite

### State Management
Tests that previously shared state through member variables need to be refactored to use static members or other shared storage mechanisms, as demonstrated in the `MigratingSharedState.cpp` example.

