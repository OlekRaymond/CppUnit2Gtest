cmake_minimum_required(VERSION 3.20..3.28.3)

project(CppUnit2Gtest_ExternalTests
    VERSION 0.0.1
    DESCRIPTION "Runs repo tests using CppUnit2Gtest instead of cppunit"
)

include(FetchContent)
include(../GetGtest.cmake)

if (NOT TARGET CppUnit2Gtest::CppUnit2Gtest)
    # find_package on our library
    find_package(CppUnit2Gtest REQUIRED)
endif()

if (NOT TARGET cppunit)
    set(cppunit_ROOT ${CppUnit2Gtest_ROOT})
    set(CppUnit_ROOT ${CppUnit2Gtest_ROOT})
    set(CPPUNIT_ROOT ${CppUnit2Gtest_ROOT})
    # less likely to help
    set(cppunit_DIR ${CppUnit2Gtest_DIR})
    set(CppUnit_DIR ${CppUnit2Gtest_DIR})
    set(CPPUNIT_DIR ${CppUnit2Gtest_DIR})
endif()

# Tests
enable_testing()
include(Martchus-cpp-utilities.cmake)

