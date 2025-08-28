# CMake minimum version
cmake_minimum_required(VERSION 3.14)

# Function to create cross-platform file links
function(create_file_link source_file target_file)
    # Get the directory of the target file
    get_filename_component(target_dir "${target_file}" DIRECTORY)

    # Check if source file exists
    if(NOT EXISTS "${source_file}")
        message(FATAL_ERROR "Source file ${source_file} does not exist! Cannot create symlink")
    endif()

    if(EXISTS "${target_file}")
        # Do nothing
        message(STATUS "Target file already exists, not creating ${target_file}")
    else()
        # Create the target directory if it doesn't exist
        file(MAKE_DIRECTORY "${target_dir}")
        # Create the (hard)link and if we fail, do a copy
        file(CREATE_LINK "${source_file}" "${target_file}" COPY_ON_ERROR)
        message(STATUS "Made link/file ${target_file}")
    endif()
endfunction()

function(create_header_alias source_file target_file)
    # Write an include to the target file
    # The ifndef is not strictly necessary but might reduce compile time
    file(WRITE "${target_file}" "#ifndef CPPUNIT_TO_GTEST_HEADER_ \n#include \"${source_file}\"\n#endif\n")
endfunction()
