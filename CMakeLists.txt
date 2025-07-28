cmake_minimum_required(VERSION 3.2...3.31)
project(foobar)

option(build_testing "Should we build our examples and tests" OFF)

if(build_testing)
    enable_testing()
    add_subdirectory(tests)
endif()

# install

# install the header as expected
# allow usage of header as if it was cppunit
# optionally install gtest as well
