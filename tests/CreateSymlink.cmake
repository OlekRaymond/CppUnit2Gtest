# CMake minimum version
cmake_minimum_required(VERSION 3.14)

# Function to create cross-platform file links
function(create_file_link source_file target_file)
    # Get the directory of the target file
    get_filename_component(target_dir "${target_file}" DIRECTORY)

    # Check if source file exists
    if(NOT EXISTS "${SOURCE_FILE}")
        message(FATAL_ERROR "Source file ${SOURCE_FILE} does not exist! Cannot create symlink")
    endif()

    # Create the target directory if it doesn't exist
    file(MAKE_DIRECTORY "${target_dir}")

    if(EXISTS "${TARGET_FILE}")
        # Do nothing
    elseif(WIN32)
        # Windows: Create symbolic link
        message(STATUS "Creating symbolic link on Windows: ${source_file} -> ${target_file}")
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E create_symlink "${source_file}" "${target_file}"
                RESULT_VARIABLE result
        )
        if(NOT result EQUAL 0)
            message(WARNING "Failed to create symbolic link. Falling back to file copy.")
            file(COPY "${source_file}" DESTINATION "${target_dir}")
            get_filename_component(target_name "${target_file}" NAME)
            get_filename_component(source_name "${source_file}" NAME)
            file(RENAME "${target_dir}/${source_name}" "${target_file}")
        endif()
    else()
        # Linux/Unix: Create hard link
        message(STATUS "Creating hard link on Linux: ${source_file} -> ${target_file}")
        execute_process(
                COMMAND ln "${source_file}" "${target_file}"
                RESULT_VARIABLE result
        )
        if(NOT result EQUAL 0)
            message(WARNING "Failed to create hard link. Falling back to file copy.")
            file(COPY "${source_file}" DESTINATION "${target_dir}")
            get_filename_component(source_name "${source_file}" NAME)
            get_filename_component(target_name "${target_file}" NAME)
            if(NOT source_name STREQUAL target_name)
                file(RENAME "${target_dir}/${source_name}" "${target_file}")
            endif()
        endif()
    endif()
endfunction()
