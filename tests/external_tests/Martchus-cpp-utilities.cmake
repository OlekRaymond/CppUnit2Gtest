
set(BUILD_TESTING ON)
set(EXCLUDE_TESTS_FROM_ALL OFF)

if (WIN32)
    set(USE_ICONV OFF)
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
