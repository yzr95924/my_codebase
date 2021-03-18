cmake_minimum_required(VERSION 3.1)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_MODULE_PATH /usr/share/cmake-3.10/Modules/)
list(APPEND CMAKE_MODULE_PATH ${BasicDedup_SOURCE_DIR}/cmake)
message(STATUS "CMAKE MODULE Path: ${CMAKE_MODULE_PATH}")


set(EXECUTABLE_OUTPUT_PATH ${MyProject_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${MyProject_SOURCE_DIR}/lib)

add_subdirectory(src)