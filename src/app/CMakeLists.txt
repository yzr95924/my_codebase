add_executable(CryptoTest crypto_test.cc)
target_link_libraries(CryptoTest ${FINAL_OBJ})

add_executable(CompressTest compress_test.cc)
target_link_libraries(CompressTest ${FINAL_OBJ})

add_executable(client client.cc)
target_link_libraries(client ${FINAL_OBJ})

add_executable(server server.cc)
target_link_libraries(server ${FINAL_OBJ})

add_executable(ChunkingTest chunking_test.cc)
target_link_libraries(ChunkingTest ${FINAL_OBJ})

add_executable(RabinTest rabin_test.cc)
target_link_libraries(RabinTest ${FINAL_OBJ})

add_executable(MQTest mq_test.cc)
target_link_libraries(MQTest ${FINAL_OBJ})

add_executable(MainTest main_test.cc)
target_link_libraries(MainTest ${FINAL_OBJ})

add_executable(RedisTest hiredis_test.cc)
target_link_libraries(RedisTest ${FINAL_OBJ})