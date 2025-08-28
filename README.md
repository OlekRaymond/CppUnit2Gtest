[![CI](https://github.com/OlekRaymond/CppUnit2Gtest/actions/workflows/CI.yml/badge.svg)](https://github.com/OlekRaymond/CppUnit2Gtest/actions/workflows/CI.yml)
[![Test Coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/wiki/OlekRaymond/CppUnit2Gtest/coverage-badge-data.md)](https://github.com/OlekRaymond/CppUnit2Gtest/actions/workflows/CI.yml)
[![Mutation Testing](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/wiki/OlekRaymond/CppUnit2Gtest/mutation-badge-data.md)](https://mull.readthedocs.io/en/0.26.0/MutationTestingIntro.html)

# CppUnit to Gtest

A C++ adapter library that allows CppUnit tests to run as if Google Tests (gtests) without requiring any test rewrites.

## Quick Start

### CMake (Recommended):
We provide a CMake package that shadows CppUnit's CMake package for seamless integration.

**Installation:**
1. `cmake -B build -S .`
2. `cmake --build build`
3. `cmake --install build`

**Usage in your project:**
```cmake
find_package(CppUnit2Gtest REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE CppUnit2Gtest::CppUnit2Gtest)
find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest) 
```
This automatically provides `CppUnit2Gtest.hpp` and `cppunit/extensions/HelperMacros.h`, shadowing common CppUnit headers. 

### Other build systems
If you're not using CMake, please create an issue requesting support for your build system.

**Manual Instructions**:
1. Copy the header file `CppUnit2Gtest.hpp` from the root of this repository
2. Replace CppUnit headers in your tests - preferably using symlinks to maintain compatibility
3. Link your tests with Google Test instead of CppUnit
4. Edit your `main` function to remove CppUnit requirements (or delete it and link to gtest's `main` lib)
5. Run your tests and see them work without modification


## What is CppUnit?
CppUnit is a C++ testing framework inspired by the Java testing framework [JUnit](https://junit.org/). 
It was widely used in the early 2000s but has since been superseded by modern frameworks like 
[Catch2](https://github.com/catchorg/Catch2) and [Google Test](https://github.com/google/googletest).

CppUnit has been significantly forked and fractured. 
The most up-to-date version is available [here](https://freedesktop.org/wiki/Software/cppunit/),
though the [SourceForge fork](https://sourceforge.net/projects/cppunit/) is also commonly used.

## Why Migrate from CppUnit?

While CppUnit served its purpose well, modern testing frameworks like Google Test offer significant advantages.

### Memory Management Issues
This project was primarily motivated by CppUnit's memory management problems, which cause false positives 
in leak sanitizers. While workarounds exist, managing extensive blacklists for multiple files becomes unwieldy.

### Google Test Advantages

**Better Test Execution**
- Built-in timing for individual tests with clear output
- Superior exception handling, including platform-specific exceptions (e.g., Windows SEH exceptions)
- Comprehensive error catching that prevents program termination

**Improved Output and Reporting**
- Color-coded output (green for pass, red for fail) with terminal support
- Detailed failure messages with line numbers and reasoning
- Native JUnit XML output format for CI/CD integration
- Rich assertion macros with informative failure messages

**Developer Experience**
- Modern C++ features and idioms
- Extensive documentation and community support
- Active development and maintenance


## Why not rewrite all my tests instead of using this?
**You should eventually!** 

For large codebases, this adapter provides a migration path that allows you to:

- Maintain existing CppUnit tests while **gradually** migrating to Google Test
- Validate that migrated tests produce identical results
- Avoid the risk and cost of rewriting large test suites all at once

More information on migrating can be found in the [examples](tests/examples/README.md) folder.

For automated migration, consider writing a Python script to handle mechanical transformations.

[//]: # (TODO: Write said python script and add it to the repo)

## Features

We focus on supporting the most commonly used CppUnit features rather than achieving 100% compatibility. 
If you need additional functionality, please create an issue with an example test case demonstrating the missing feature.

We currently support these features:
- All features demonstrated in the official CppUnit repository examples, as [seen here](./tests/examples).
- Adding tests using CppUnit macros (`CPPUNIT_TEST` and `CPPUNIT_TEST_EXCEPTION` after `CPPUNIT_TEST_SUITE` or `CPPUNIT_TEST_SUB_SUITE`)
- Registering using CppUnit's macros (`CPPUNIT_TEST_SUITE_REGISTRATION` or `CPPUNIT_TEST_SUITE_NAMED_REGISTRATION` must be called to register tests)
- CppUnit's specialized assertion macros, allowing custom messages (or using gtest's streams)

## Contributing

**Summary:** Create an issue, fork the repo, submit a PR, ensure CI passes, and wait patiently for review.
See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

## License

**Summary:** Attribution required for redistribution; commercial modifications must be made public or contributed back.
See [LICENSE](./LICENSE) for full details.
