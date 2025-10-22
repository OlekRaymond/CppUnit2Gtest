cmake_minimum_required(VERSION 3.20..3.28.3)

project(CppUnit2Gtest_ExternalTests
    VERSION 0.0.0
    DESCRIPTION "Adapts CppUnit tests to run under the gtest framework"
)

include(FetchContent)
include(../GetGtest.cmake)

# find_package on our library
find_package(CppUnit2Gtest REQUIRED)

# Alias it to have CMake understand
add_library(cppunit ALIAS CppUnit2Gtest::CppUnit2Gtest)

# Tests
enable_testing()
include(Martchus-cpp-utilities.cmake)

