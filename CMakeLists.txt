#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")
cmake_minimum_required(VERSION 3.1)
project(MyCodeBase)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_MODULE_PATH /usr/share/cmake-3.10/Modules/)
list(APPEND CMAKE_MODULE_PATH ${MyCodeBase_SOURCE_DIR}/cmake)
message(STATUS "CMAKE MODULE Path: ${CMAKE_MODULE_PATH}")


set(EXECUTABLE_OUTPUT_PATH ${MyCodeBase_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${MyCodeBase_SOURCE_DIR}/lib)

set(CMAKE_C_FLAGS_DEBUG "-Wall -g -ggdb") 
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -ggdb -fsanitize=address -O0 -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -ggdb") # for gdb, for valgrind
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

SET (CMAKE_C_COMPILER "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_AR "/usr/bin/llvm-ar")
SET (CMAKE_LINKER "/usr/bin/llvm-ld")
SET (CMAKE_NM "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB "/usr/bin/llvm-ranlib")

add_subdirectory(src)