
set(BUILD_TESTING ON)
set(EXCLUDE_TESTS_FROM_ALL OFF)

if (WIN32)
    # Attempts to reduce features to allow it to work on CI
    set(USE_ICONV OFF)
    set(USE_BOOST_PROCESS OFF)
    set(USE_NATIVE_FILE_BUFFER ON)
    set(FORCE_BOOST_IOSTREAMS_FOR_NATIVE_FILE_BUFFER OFF)
    # Either boost or gcc is needed so it doesn't work on CI
    if ("$ENV{CI}")
        message(STATUS "Cannot build cpp-utilites without boost or gnu, skipping test")
        return()
    endif()
endif()

# pull the code we want to test
FetchContent_Declare(
    cpputils
    GIT_REPOSITORY https://github.com/OlekRaymond/cpp-utilities.git
    GIT_TAG        cppunit-removal
)
# Currently using a fork with small changes.
#  TODO: update to use source repo and release tags

# Make it available
#  This also includes the build for the lib which I don't think we need
FetchContent_MakeAvailable(cpputils)

# Use ctest to run
#  (nothing required)
