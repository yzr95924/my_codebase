add_executable(chunkingBench chunkingBench.cc)
add_executable(fastCDC fastTest.cc)

target_link_libraries(chunkingBench ${FINAL_OBJ})
target_link_libraries(fastCDC ${FINAL_OBJ})

add_executable(cryptoBench cryptoTest.cc)
target_link_libraries(cryptoBench ${FINAL_OBJ})

add_executable(compressTest compressTest.cc)
target_link_libraries(compressTest ${FINAL_OBJ})

add_executable(client client.cc)
target_link_libraries(client ${FINAL_OBJ})

add_executable(server server.cc)
target_link_libraries(server ${FINAL_OBJ})

add_executable(main tmp_main.cc)
target_link_libraries(main ${OUTSIDE_OBJ} boostTutorLib)