# Examples

This folder contains examples how to use CppUnit2Gtest. Most examples are taken from CppUnit's repo and refactored into a single file. These are: [Hierarchy.cpp](Hierarchy.cpp), [Money.cpp](Money.cpp) and [Simple.cpp](Simple.cpp). 

The migration examples demonstrate how to incrementally transition from CppUnit to Google Test using CppUnit2Gtest. Each example is self-contained in a single file with all tests passing.

## Migration Summary

Migrations of most tests should be as simple as removing the CppUnit CMake install and replacing it with a CppUnit2Gtest one, then removing the main function and replacing it with a gtest compatable main. (In the given examples we link to the Google supplied main lib.)

## Migration Philosophy

All migration approaches are designed to be **incremental**, allowing you to maintain codebases with **both Google Test and CppUnit-style** tests running **side by side**. This means you can migrate your test suite **gradually** without breaking existing functionality.

## Migration Examples

Two comprehensive migration examples are provided, each one contains different stages of the migration process.

### 1. [Migration.cpp](Migration.cpp)
Shows the migration pattern for tests that don't share state between test cases:
- **Stage 1**: Original CppUnit tests
- **Stage 2**: Mixed CppUnit/Google Test using the adapter
- **Stage 3**: Pure Google Test implementation

### 2. [MigratingSharedState.cpp](MigratingSharedState.cpp)
This example demonstrates more complex handling of state shared between tests:
- Database connections, or other costly setup operations
- Demonstrates proper use of `SetUpTestSuite()` and `TearDownTestSuite()`
- Shows code changes required to handle the difference in constructor behavior

**Best for**: Integration tests or tests with expensive setup/teardown operations.

## Common Migration Challenges

### Constructors
**Both** CppUnit and Google Test class constructors run once per individual test.

In Google Test, setup logic can be shared with Google Test's static lifecycle methods:
- `SetUpTestSuite()` - runs once before all tests in the suite
- `TearDownTestSuite()` - runs once after all tests in the suite
