add_executable(chunkingBench chunkingBench.cc)
add_executable(fastCDC fastTest.cc)

target_link_libraries(chunkingBench ${FINAL_OBJ})
target_link_libraries(fastCDC ${FINAL_OBJ})

add_executable(cryptoBench cryptoTest.cc)
target_link_libraries(cryptoBench ${FINAL_OBJ})

add_executable(compressTest compressTest.cc)
target_link_libraries(compressTest ${FINAL_OBJ})

add_executable(heapTest heapTest.cc)
target_link_libraries(heapTest ${FINAL_OBJ})