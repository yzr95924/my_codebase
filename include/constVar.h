/**
 * @file constVar.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the const variables
 * @version 0.1
 * @date 2022-02-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef EDRSTORE_CONST_VAR_H
#define EDRSTORE_CONST_VAR_H

#include <bits/stdc++.h>

// client side breakdown
#define CHUNKING_BREAKDOWN 1

// server side breakdown

// chunking method
static const int CHUNKER_FIX_SIZE_TYPE = 0; // for the type of fixed-size chunker
static const int CHUNKER_VAR_SIZE_TYPE = 1; // for the type of variable-size chunker
static const int FAST_CDC = 2;

// chunk type
static const size_t MAX_CHUNK_SIZE = 16384;

static const uint32_t CHUNK_QUEUE_SIZE = (1024 * 4);
static const size_t THREAD_STACK_SIZE = (8*1024*1024);

// sketch & super-feature & feature settings
static const uint32_t SUPER_NUM_PER_CHUNK = 3; // 3 super-feature per sketch
static const uint32_t FEATURE_NUM_PER_SUPER = 4; // 4 features per super-feature
static const uint32_t FEATURE_NUM_PER_CHUNK = SUPER_NUM_PER_CHUNK * FEATURE_NUM_PER_SUPER;

// for rabin fingerprint
static const uint64_t FINGERPRINT_PT = 0xbfe6b8a5bf378d83LL;

// data type enum
enum DATA_TYPE {DATA_CHUNK = 0, RECIPE_END};

// for crypto info 
enum ENCRYPT_SET {AES_256_GCM = 0, AES_128_GCM, AES_256_CFB, AES_128_CFB, 
    AES_256_CTR, AES_128_CTR, AES_256_ECB, AES_128_ECB};
enum HASH_SET {SHA_256 = 0, MD5 = 1, SHA_1 = 2};
static const uint32_t CRYPTO_BLOCK_SIZE = 16;
static const uint32_t CHUNK_HASH_SIZE = 32;
static const int HASH_TYPE = SHA_256;
static const int CIPHER_TYPE = AES_256_CTR;

// the setting of the container
static const uint32_t MAX_CONTAINER_SIZE = (1 << 22); // container size: 4MB
static const uint32_t CONTAINER_ID_LENGTH = 8; // 8 bytes

// the operation type
enum CLIENT_OPT_TYPE {UPLOAD_OPT = 0, RESTORE_OPT};

// for SSL connection
static const int IN_SERVERSIDE = 0;
static const int IN_CLIENTSIDE = 1;

// for network protocol
enum NETWORK_PROTOCOL {CLIENT_UPLOAD_LOGIN = 0, CLIENT_RESTORE_LOGIN, 
    SEND_CHUNK, SEND_NORMAL_RECIPE, SEND_RECIPE_END,
    REPLY_NOT_EXIST, REPLY_LOGIN, RESTORE_RECIPE, 
    RESTORE_RECIPE_REPLY, RESTORE_CHUNK, CLIENT_RESTORE_READY};

enum CHUNK_STATUS {UNIQUE = 0, DUPLICATE, SIMILAR, NON_SIMILAR, COMPRESSED_CHUNK,
    UNCOMPRESSED_CHUNK};

// for SSL connection 
static const char SERVER_CERT[] = "../key/server/server.crt";
static const char SERVER_KEY[] = "../key/server/server.key";
static const char CLIENT_CERT[] = "../key/client/client.crt";
static const char CLIENT_KEY[] = "../key/client/client.key";
static const char CA_CERT[] = "../key/ca/ca.crt";

// max client
static const int MAX_CLIENT_NUM = 10;

#endif