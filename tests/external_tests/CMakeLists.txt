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

# Alias it to have CMake understand
if (NOT TARGET cppunit)
    add_library(cppunit ALIAS CppUnit2Gtest::CppUnit2Gtest)
endif()

# Tests
enable_testing()
include(Martchus-cpp-utilities.cmake)

