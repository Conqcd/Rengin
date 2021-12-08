cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 17)
project(Rengin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_definitions(-DRE_DEBUG)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release")) 
    add_definitions(-DRE_RELEASE)
endif()

add_subdirectory(rengine-src)
add_subdirectory(SandBox)