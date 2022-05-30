add_executable(CryptoTest cryptoTest.cc)
target_link_libraries(CryptoTest ${FINAL_OBJ})

add_executable(CompressTest compressTest.cc)
target_link_libraries(CompressTest ${FINAL_OBJ})

add_executable(client client.cc)
target_link_libraries(client ${FINAL_OBJ})

add_executable(server server.cc)
target_link_libraries(server ${FINAL_OBJ})