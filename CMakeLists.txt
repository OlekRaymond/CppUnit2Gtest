cmake_minimum_required(VERSION 3.2...3.31)
project(
    CppUnit2Gtest
    VERSION 0.0.0
    DESCRIPTION "Adapts CppUnit tests to run under the gtest framework"
    LANGUAGES CXX
)

option(build_testing "Should we build our examples and tests" OFF)
option(EnableMainLikeClasses "Enable main-like classes for CppUnit2Gtest" OFF)

if(build_testing)
    enable_testing()
    add_subdirectory(tests)
endif()

# Empty targets (for installing)
add_library(CppUnit2Gtest INTERFACE)
add_library(CppUnit2Gtest::CppUnit2Gtest ALIAS CppUnit2Gtest)
if (EnableMainLikeClasses)
    target_compile_definitions(CppUnit2Gtest INTERFACE Cpp2Unit2Gtest_EnableMainHelperClasses)
endif()

# Set include directories
target_include_directories(CppUnit2Gtest INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# install

# Install targets
install(TARGETS CppUnit2Gtest
    EXPORT CppUnit2GtestTargets
    INCLUDES DESTINATION include
)

install(EXPORT CppUnit2GtestTargets
    FILE CppUnit2GtestTargets.cmake
    NAMESPACE CppUnit2Gtest::
    DESTINATION "share/cmake/${PROJECT_NAME}"
)

include(CMakePackageConfigHelpers)

# Create a config file that CMake looks for when we call find_package(core_project)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "share/cmake/${PROJECT_NAME}"
)

write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "share/cmake/${PROJECT_NAME}"
)

# install the header as expected
install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/CppUnit2Gtest.hpp"
    TYPE INCLUDE
)

include(GNUInstallDirs)

# allow usage of headers as if it was cppunit
install(CODE "
set(INSTALL_INCLUDE_DIR \${CMAKE_INSTALL_PREFIX}/include)
set(SOURCE_FILE CppUnit2Gtest.hpp)
include(tests/CreateSymlink.cmake)
create_header_alias(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/portability/Stream.h)
create_header_alias(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/extensions/HelperMacros.h)
create_header_alias(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/config/SourcePrefix.h)
create_header_alias(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/TestAssert.h)
")

# We don't install GTest so the user can have any they would like and/or install it later
