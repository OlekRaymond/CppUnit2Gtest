# TESTING
#  This directory contains tests
#    examples (all tested) are contained within the examples directory
#    other tests are contained within the internal_tests directory
#       (and are not intended to be used as examples)

cmake_minimum_required(VERSION 3.2...3.33)

project(CppUnit2Gtest_AllTests)

option(BuildExamples      "Builds all the examples in ./examples" ON)
option(BuildInternalTests "Builds the internal tests" ON)
option(BuildUnityTests 
    "Build lots of tests into one large file with expanded preprocessor, aimed for mutation tests which do not mutate macros"
    OFF
)
option(MutationTesting "Experimental, for mutation testing" OFF)

if (NOT (BuildExamples OR BuildInternalTests OR BuildUnityTests))
    message(FATAL_ERROR "Asked to build tests but no tests were given")
endif()

set(CMAKE_CXX_STANDARD 17)
if (BuildExamples)
    list(APPEND CppUnitFiles
        "examples/Money.cpp"
        "examples/Simple.cpp"
        "examples/Hierarchy.cpp"
        "examples/Migrating.cpp"
        "examples/MigratingSharedState.cpp"
    )
endif()
if (BuildInternalTests)
    list(APPEND CppUnitFiles
        "internal_tests/DeprecatedDeclareThrow.cpp"
        "internal_tests/CheckFailures.cpp"
        "internal_tests/RegisteringTests.cpp"
        "internal_tests/TestTestData.cpp"
        "internal_tests/TestGettingData.cpp"
        "internal_tests/TestMainClasses.cpp"
    )
endif()
if (BuildUnityTests)
    # Call gcc with -E to get a preprocessed file but with no external headers then add the headers and compile again
    
    # We can't use these includes before preprocessing
    #  hold them to prepend after preprocessing
    set(GtestTempHeaderInclude "${CMAKE_CURRENT_BINARY_DIR}/gtest_headers.txt")
    file(WRITE "${GtestTempHeaderInclude}" "#include <gtest/gtest.h>\n#include <gtest/gtest-spi.h>\n")

    #  Get the preprocessed file and compile against that
    set(UnityTestSrc "${CMAKE_CURRENT_BINARY_DIR}/AllTestsUnity_NoPP.cpp")                  # Before preprocessing
    set(UnityTestPreprocessed "${CMAKE_CURRENT_BINARY_DIR}/AllTestsUnity_PP.cpp")           # After preprocessing
    set(UnityTestPreprocessedIncludes "${CMAKE_CURRENT_BINARY_DIR}/AllTestsUnity_PPI.cpp")  # After preprocessing with includes

    # Read our main header into a variable
    file(READ "${CMAKE_CURRENT_LIST_DIR}/../CppUnit2Gtest.hpp" CPPUNIT2GTEST_CONTENTS)
    # Remove all includes
    string(REGEX REPLACE "#include <[A-Za-z0-9\/\.]*>" "" CPPUNIT2GTEST_CONTENTS "${CPPUNIT2GTEST_CONTENTS}")
    # Manual appending our own, modified header and copy it into UnityTestSrc 
    configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/internal_tests/AllTestsUnity.cpp.in"
        "${UnityTestSrc}"
        @ONLY
    )
    # Preprocess the file to expand all macros, output the preprocessed file
    add_custom_command(
        OUTPUT "${UnityTestPreprocessed}"
        COMMAND ${CMAKE_CXX_COMPILER} -E -P "${UnityTestSrc}" -o "${UnityTestPreprocessed}"
        DEPENDS "${UnityTestSrc}"
        COMMENT "Preprocessing AllTestsUnity.cpp to AllTestsUnity_PP.cpp"
    )
    # Add back the includes it needs to compile.
    #   Needs to be custom command (must be done after the previous custom command so the file exists)
    add_custom_command(
        OUTPUT "${UnityTestPreprocessedIncludes}"
        COMMAND ${CMAKE_COMMAND} -E cat 
            "${GtestTempHeaderInclude}" 
            "${UnityTestPreprocessed}" 
            > "${UnityTestPreprocessedIncludes}"
        DEPENDS "${UnityTestPreprocessed}"
    )
    # Add the eventaul file so we run the tests against it.
    list(APPEND CppUnitFiles
        ${UnityTestPreprocessedIncludes}
    )
endif()

include(GetGtest.cmake)
add_executable(${PROJECT_NAME} ${CppUnitFiles})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest GTest::Main)

if (EnableMainHelperClasses)
    target_compile_definitions(${PROJECT_NAME} PUBLIC Cpp2Unit2Gtest_EnableMainHelperClasses)
endif()

if (BuildWithCoverage)
    if (MSVC)
        message(FATAL_ERROR "Cannot build with coverage on MSVC, please use GCC")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
        target_link_options(${PROJECT_NAME} PRIVATE --coverage)
    endif()
endif()

if (NOT build_testing)
    message(STATUS "Building tests as if external user by using find_package calls")
    find_package(CppUnit2Gtest REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE CppUnit2Gtest::CppUnit2Gtest)
else()
    message(STATUS "Building tests as internal tests (without find_package calls)")

    # Create links to our main file so they can be used by cppunit style imports
    include(CreateSymlink.cmake)
    set(SOURCE_FILE "../CppUnit2Gtest.hpp")

    # Allow including files that cppunit has
    create_header_alias("${SOURCE_FILE}" "${CMAKE_CURRENT_LIST_DIR}/cppunit/extensions/HelperMacros.h")
    create_header_alias("${SOURCE_FILE}" "${CMAKE_CURRENT_LIST_DIR}/cppunit/config/SourcePrefix.h")
    create_header_alias("${SOURCE_FILE}" "${CMAKE_CURRENT_LIST_DIR}/cppunit/portability/Stream.h")
    
    # Add headers for IDEs
    list(APPEND CppUnitFiles
        "cppunit/extensions/HelperMacros.h"
        "cppunit/portability/Stream.h"
        "cppunit/config/SourcePrefix.h"
        "${SOURCE_FILE}"
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
# The gtest find tests function doesn't work because it relies on regex expressions
#  We have tests that look like CppUnit so it won't add them.
# It's a lot easier to just run the tests directly
enable_testing()
add_test(NAME AllTests COMMAND ${PROJECT_NAME})
