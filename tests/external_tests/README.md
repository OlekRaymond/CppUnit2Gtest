# External tests

These tests are on external projects and follow roughly these steps for drop in replacement tests:

- Build CppUnit2Gtest
    + Requires `-DEnableMainLikeClasses=ON -DInstallAsCppUnit=ON`
- Install CppUnit2Gtest
    + To prefx path "P"
- Build 3rd Party tests
    + Telling CMake to look in "P" for CppUnit
- Run tests

Or for repos where we have changed main:
- Build CppUnit2Gtest
- Install CppUnit2Gtest
- Build 3rd Party tests
- Run tests


