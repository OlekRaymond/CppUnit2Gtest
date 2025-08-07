# TESTING
#  This directory contains tests
#    examples (all tested) are contained within the examples directory
#    other tests are contained within the internal_tests directory
#       (and are not intended to be used as examples)

cmake_minimum_required(VERSION 3.2...3.33)

project(CppUnit2Gtest_AllTests)

option(BuildExamples "Should the examples be built when build_testing is on" ON)
option(BuildInternalTests "Should we build the internal tests" OFF)

if (NOT (BuildExamples OR BuildInternalTests))
    message(FATAL_ERROR "Asked to build tests but no tests were given")
endif()

set(CMAKE_CXX_STANDARD 17)
if (BuildExamples)
    list(APPEND CppUnitFiles
        "examples/Money.cpp"
        "examples/Simple.cpp"
        "examples/Hierarchy.cpp"
    )
endif()
if (BuildInternalTests)
    list(APPEND CppUnitFiles
        "internal_tests/DeprecatedDeclareThrow.cpp"
        "internal_tests/CheckFailures.cpp"
        "internal_tests/RegisteringTests.cpp"
        "internal_tests/TestTestData.cpp"
        "internal_tests/TestGettingData.cpp"
    )
endif()

include(GetGtest.cmake)
add_executable(${PROJECT_NAME} ${CppUnitFiles})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest GTest::Main)

if (NOT build_testing)
    message(STATUS "Building tests as if external user by using find_package calls")
    find_package(CppUnit2Gtest REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE CppUnit2Gtest::CppUnit2Gtest)
else()
    message(STATUS "Building tests as internal tests (without find_package calls)")

    # Create links to our main file so they can be used by cppunit style imports
    include(CreateSymlink.cmake)
    set(SOURCE_FILE "${CMAKE_CURRENT_LIST_DIR}/../CppUnit2Gtest.hpp")

    # Allow using files that cppunit has, old style ifdef guards avoid issues with multiple defines
    create_file_link("${SOURCE_FILE}" "${CMAKE_CURRENT_LIST_DIR}/cppunit/portability/Stream.h")
    create_file_link("${SOURCE_FILE}" "${CMAKE_CURRENT_LIST_DIR}/cppunit/extensions/HelperMacros.h")
    create_file_link("${SOURCE_FILE}" "${CMAKE_CURRENT_LIST_DIR}/cppunit/config/SourcePrefix.h")
    # Add headers for IDEs
    list(APPEND CppUnitFiles
        "cppunit/extensions/HelperMacros.h"
        "cppunit/portability/Stream.h"
        "cppunit/config/SourcePrefix.h"
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
enable_testing()
add_test(NAME AllTests COMMAND ${PROJECT_NAME})
