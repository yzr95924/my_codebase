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

#endif //  CHUNKING_LIB_CONFIGURE!

