cmake_minimum_required(VERSION 3.2...3.31)
project(
    CppUnit2Gtest
    VERSION 0.0.0
    DESCRIPTION "Adapts CppUnit tests to run under the gtest framework"
    LANGUAGES CXX
)

option(build_testing "Should we build our examples and tests" OFF)

if(build_testing)
    enable_testing()
    add_subdirectory(tests)
endif()

# install

install(
    FILES "${CMAKE_CURRENT_SOURCE_DIR}/CppUnit2Gtest.hpp"
    TYPE INCLUDE
)
# foreach ( _cppUnitFile CPPUNIT_FILES )
    # install (
        # FILES "${CMAKE_CURRENT_SOURCE_DIR}/CppUnit2Gtest.hpp"
        # TYPE INCLUDE
        # RENAME "cppunit/${_cppUnitFile}.hpp"
    # )
# endforeach
include(CMakePackageConfigHelpers)

# Create a config file that CMake looks for when we call FindPackage(core_project)
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
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION
        share/cmake/${PROJECT_NAME}
)

# install the header as expected
# allow usage of header as if it was cppunit
# optionally install gtest as well
