find_package(Boost 1.36.0 REQUIRED COMPONENTS thread system serialization)
find_package(OpenSSL REQUIRED)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    link_directories(${OPENSSL_LIBRARIES})
endif()

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ../include)
link_directories(${Boost_LIBRARY_DIRS} ${OPENSSL_LIBRARIES} ../lib)


set(BOOST_LIBRARY_OBJ boost_thread boost_system boost_serialization)
set(SYSTEM_LIBRARY_OBJ pthread)
set(CHUNKING_OBJ rfChunking fixChunking fastChunking fastCDCTest)
set(UTIL_OBJ utilCore)
set(OPENSSL_OBJ ssl crypto)
set(CRYPTO_OBJ cryptoTool)

set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "-Wall -std=c17 -g -ggdb -g2") 
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -std=c++17 -g -ggdb -g2 -fsanitize=leak -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_RELEASE "-O3 -std=c17") 
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++17")
set(CMAKE_MODULE_PATH /usr/share/cmake-3.10/Modules/)

SET (CMAKE_C_COMPILER "/usr/bin/clang")
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_AR "/usr/bin/llvm-ar")
SET (CMAKE_LINKER "/usr/bin/llvm-ld")
SET (CMAKE_NM "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB "/usr/bin/llvm-ranlib")

add_subdirectory(multhread)
add_subdirectory(network)
add_subdirectory(test)
add_subdirectory(chunking)
add_subdirectory(util)
add_subdirectory(crypto)
