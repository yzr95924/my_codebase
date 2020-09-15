add_executable(chunkingBench chunkingBench.cc)

target_link_libraries(chunkingBench ${CHUNKING_OBJ} ${UTIL_OBJ})