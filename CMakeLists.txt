cmake_minimum_required(VERSION 3.1)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE TRUE)


set(EXECUTABLE_OUTPUT_PATH ${MyProject_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${MyProject_SOURCE_DIR}/lib)

add_subdirectory(src)