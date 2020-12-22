add_executable(chunkingBench chunkingBench.cc)
add_executable(fastCDC fastTest.cc)

target_link_libraries(chunkingBench ${CHUNKING_OBJ} ${UTIL_OBJ})
target_link_libraries(fastCDC ${CHUNKING_OBJ} ${UTIL_OBJ})

add_executable(cryptoBench cryptoTest.cc)
target_link_libraries(cryptoBench ${OPENSSL_OBJ} ${UTIL_OBJ} ${CRYPTO_OBJ})

add_executable(compressTest compressTest.cc)
target_link_libraries(compressTest ${COMPRESS_OBJ} ${UTIL_OBJ} lz4)

add_executable(heapTest heapTest.cc)
target_link_libraries(heapTest ${UTIL_OBJ})