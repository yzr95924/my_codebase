find_package(Boost 1.36.0 REQUIRED COMPONENTS thread system serialization)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include_directories(${Boost_INCLUDE_DIRS} ../include)
link_directories(${Boost_LIBRARY_DIRS} ../lib)


set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "-Wall -std=c1x -g -ggdb -g2") 
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -std=c++17 -g -ggdb -g2 -fsanitize=leak -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS_DEBUG "-Wall -std=c++17 -g -ggdb -g2 -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELEASE "-O3 -std=c1x") 
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11")
set(CMAKE_MODULE_PATH /usr/share/cmake-3.10/Modules/)

SET (CMAKE_C_COMPILER "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_AR "/usr/bin/llvm-ar")
SET (CMAKE_LINKER "/usr/bin/llvm-ld")
SET (CMAKE_NM "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB "/usr/bin/llvm-ranlib")
