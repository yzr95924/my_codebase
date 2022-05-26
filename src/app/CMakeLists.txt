add_executable(chunkingBench chunkingBench.cc)

target_link_libraries(chunkingBench ${FINAL_OBJ})

add_executable(cryptoBench cryptoTest.cc)
target_link_libraries(cryptoBench ${FINAL_OBJ})

add_executable(compressTest compressTest.cc)
target_link_libraries(compressTest ${FINAL_OBJ})

add_executable(client client.cc)
target_link_libraries(client ${FINAL_OBJ})

add_executable(server server.cc)
target_link_libraries(server ${FINAL_OBJ})