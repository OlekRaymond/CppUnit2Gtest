# TESTING
#  This directory contains tests
#    examples (all tested) are contained within the examples directory
#    other tests are contained within the internal_tests directory
#       (and are not intended to be used as examples)

cmake_minimum_required(VERSION 3.2...3.33)

project(CppUnitToGtest_AllTests)

include(GetGtest.cmake)

option(BuildExamples "Should the examples be built when build_testing is on" ON)
option(BuildInternalTests "Should we build the internal tests" OFF)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CppUnitFiles
        "cppunit/extensions/HelperMacros.h"
        "cppunit/portability/Stream.h"
)
if (BuildExamples)
#    add_subdirectory(examples)
    list(APPEND CppUnitFiles
            "examples/Money.cpp"
            "examples/Simple.cpp"
            "examples/Hierarchy.cpp"
    )
endif()
if (BuildInternalTests)
#    add_subdirectory(internal_tests)
#    list(APPEND CppUnitFiles ${CppunitToGtestTests_SourceFiles})
endif()

add_executable(${PROJECT_NAME} ${CppUnitFiles})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest GTest::Main)

# Create links to our main file so they can be used by cppunit style imports
include(CreateSymlink.cmake)
set(SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../CppUnit2Gtest.hpp")

# Allow using files that cppunit has, old style ifdef guards avoid issues with multiple defines
create_file_link("${SOURCE_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/cppunit/portability/Stream.h")
create_file_link("${SOURCE_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/cppunit/extensions/HelperMacros.h")
create_file_link("${SOURCE_FILE}" "${CMAKE_CURRENT_SOURCE_DIR}/cppunit/config/SourcePrefix.h")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
