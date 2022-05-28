add_executable(cryptoBench cryptoTest.cc)
target_link_libraries(cryptoBench ${FINAL_OBJ})

add_executable(CompressTest compressTest.cc)
target_link_libraries(CompressTest ${FINAL_OBJ})

add_executable(client client.cc)
target_link_libraries(client ${FINAL_OBJ})

add_executable(server server.cc)
target_link_libraries(server ${FINAL_OBJ})