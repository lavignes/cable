cmake_minimum_required(VERSION 3.5)
project(cable)

# Add cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

add_subdirectory(libcable)

add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
        )
