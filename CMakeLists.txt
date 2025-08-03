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
set(SOURCE_FILE \${INSTALL_INCLUDE_DIR}/CppUnit2Gtest.hpp)
include(tests/CreateSymlink.cmake)
create_file_link(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/portability/Stream.h)
create_file_link(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/extensions/HelperMacros.h)
create_file_link(\${SOURCE_FILE} \${INSTALL_INCLUDE_DIR}/cppunit/config/SourcePrefix.h)
")


# optionally install gtest as well
