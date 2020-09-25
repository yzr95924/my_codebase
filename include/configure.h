/**
 * @file configure.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief set the configuration macro 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CHUNKING_LIB_CONFIGURE 
#define CHUNKING_LIB_CONFIGURE

#define BREAKDOWN_DEFINE 1
#define CHUNK_HASH_SIZE 32
#define MAX_CHUNK_SIZE 16384 // 16KB
#define CHUNK_ENCRYPT_KEY_SIZE 32
#define FILE_NAME_HASH_SIZE 32

#define DATA_TYPE_RECIPE 1
#define DATA_TYPE_CHUNK 2

#define RF_CHUNKING 1
#define FIX_CHUNKING 2
#define FASTCDC_CHUNKING 3


enum ENCRYPT_SET {AES_256_GCM = 0, AES_256_CFB = 1, AES_256_CBC = 2};

enum HASH_SET {SHA_1 = 0, SHA_256 = 1, MD5 = 2};

// the size cipher block: 128 bits
#define CRYPTO_BLOCK_SIZE 16

#endif //  CHUNKING_LIB_CONFIGURE!

