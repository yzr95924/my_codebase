add_executable(chunkingBench chunkingBench.cc)
add_executable(fastCDC fastTest.cc)

target_link_libraries(chunkingBench ${CHUNKING_OBJ} ${UTIL_OBJ})
target_link_libraries(fastCDC ${CHUNKING_OBJ} ${UTIL_OBJ})