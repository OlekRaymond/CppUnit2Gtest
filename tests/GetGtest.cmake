# Try to find GTest first
find_package(GTest QUIET)

# Mostly useful when testing/building pipeline since we don't want to build third party code every time
option(NeverFetchGtest "If we can't find gtest we fetch it from the internet, setting this makes the configure fail instead" OFF)

if (NeverFetchGtest AND NOT GTest_FOUND)
    message(FATAL_ERROR "GTest not found and NeverFetchGtest was set, failing configure")
elseif(NOT GTest_FOUND)
    message(STATUS "GTest not found, fetching from GitHub...")

    include(FetchContent)
    # Fetch GTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.16.0  # Use a specific tag for reproducible builds
    )
    # Options to configure GTest
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(BUILD_GTEST ON CACHE BOOL "" FORCE)

    # Make GTest available
    FetchContent_MakeAvailable(googletest)

    # Create aliases to match find_package names
    add_library(GTest::GTest ALIAS gtest)
    add_library(GTest::Main ALIAS gtest_main)
else()
    message(STATUS "Found GTest: ${GTest_VERSION}")
endif()
