/**
 * @file constVar.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the const variables 
 * @version 0.1
 * @date 2020-12-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CONST_VAR_H
#define CONST_VAR_H

// For configure show
#define SHOW_CONFIGURE 0

// For session key encryption breakdown
#define SESSION_KEY_ENCRYPTION 1

// the top-k value
#define TOP_K_PARAMETER 1024*512

// for enclave lib path
#define ENCLAVE_PATH "../lib/storeEnclave.signed.so"

// For client upload process breakdown
#define CHUNKING_BREAKDOWN 1
#define FPWORKER_BREAKDOWN 0
#define SENDER_BREAKDOWN 0
///

#define RECEIVER_BREAKDOWN 0
#define STORAGE_BREAKDOWN 0
#define INDEX_BREAKDOWN 0
#define DATAWRITER_BREAKDOWN 0
// For restore process breakdown
#define RESTORE_WRITER_BREAKDOWN 0
#define CHUNK_RETRIVER_BREAKDOWN 0
#define RECV_DECODE_BREAKDOWN 0 

// For SGX_Breakdown 
#define SGX_BREAKDOWN 0

// For whether enable restore container cache
#define CONTAINER_CACHE_ENABLE 1

// the length of the hash
#define HIGH_SECURITY 1
#define CRYPTO_BLOCK_SIZE 16

#if (HIGH_SECURITY == 1) 
#define CHUNK_FINGER_PRINT_SIZE 32
#define CHUNK_HASH_SIZE 32
#define CHUNK_ENCRYPT_KEY_SIZE 32
#define FILE_NAME_HASH_SIZE 32
#define HASH_TYPE 1
#define CIPHER_TYPE 0
#else
#define CHUNK_FINGER_PRINT_SIZE 16
#define CHUNK_HASH_SIZE 16
#define CHUNK_ENCRYPT_KEY_SIZE 16
#define FILE_NAME_HASH_SIZE 16
#define HASH_TYPE 0
#define CIPHER_TYPE 1
#endif

// the size of chunk
#define MIN_CHUNK_SIZE 4096
#define AVG_CHUNK_SIZE 8192
#define MAX_CHUNK_SIZE 16384

// the size of segment
#define AVG_SEGMENT_SIZE (1024 * 1024 * 10) // 10MB default
#define MIN_SEGMENT_SIZE (AVG_SEGMENT_SIZE / 2) // 5MB default
#define MAX_SEGMENT_SIZE (AVG_SEGMENT_SIZE * 2) // 20MB default
#define DIVISOR ((AVG_SEGMENT_SIZE - MIN_SEGMENT_SIZE) / (8 * (1 << 10)))
#define PATTERN 1
#define MAX_ALLOW_FACTOR 1

// the type of chunker
#define CHUNKER_FIX_SIZE_TYPE 0 // for the type of fixed-size chunker
#define CHUNKER_VAR_SIZE_TYPE 1 // for the type of variable-size chunker
// For trace-driven experiment
#define FSL_TRACE 2
#define UBC_TRACE 3
#define FAST_CDC 4

// the setting of the container
#define MAX_CONTAINER_SIZE (1 << 23) // container size: 8MB
#define MAX_COMPRESS_BUFFER_SIZE (1 << 24) // compress buffer size: 16MB
#define UUID_LENGTH 36

// define the data type of the MQ
#define DATA_TYPE_RECIPE 1
#define DATA_TYPE_CHUNK 2
#define DATA_TYPE_HEAD 3
#define DATA_SEGMENT_END_FLAG 4

// configure for sparse index
#define SPARSE_INDEX_SAMPLE_RATE 6
#define SPARSE_INDEX_CHAMPION_NUM 10
#define SPARSE_INDEX_MANIFIEST_CAP_NUM 10

#define SKETCH_DEPTH 4

// for performance measurement
#define SEC_TO_USEC 1000000.0

enum ENCRYPT_SET {AES_256_GCM = 0, AES_128_GCM, AES_256_CFB, AES_128_CFB, 
    AES_256_CBC, AES_128_CBC, AES_256_CTR, AES_128_CTR};

enum HASH_SET {SHA_1 = 0, SHA_256 = 1, MD5 = 2};

enum ENABLE_SGX_FLAG {SGX_disable = 0, SGX_enable = 1};

enum INDEX_TYPE_SET {SIMPLE = 0, EXTREME_BINNING = 1, SPARSE_INDEX = 2, FREQ_INDEX = 3, FREQ_TWO_INDEX = 4, ENCLAVE_BASIC = 5};


// for SSL connection
#define IN_SERVERSIDE 0
#define IN_CLIENTSIDE 1

// for SSL connection 
#define SERVER_CERT "../key/server/server.crt"
#define SERVER_KEY "../key/server/server.key"
#define CLIENT_CERT "../key/client/client.crt"
#define CLIENT_KEY "../key/client/client.key"
#define CA_CERT "../key/ca/ca.crt"

// for network message protocol code 
enum PROTCOL_CODE_SET {CLIENT_UPLOAD_CHUNK = 0, CLIENT_UPLOAD_HEADER, CLIENT_UPLOAD_RECIPE, CLIENT_EXIT, 
    RESTORE_REQUEST, RESTORE_DATA, RESTORE_FINAL, CLIENT_LOGIN_UPLOAD, CLIENT_LOGIN_RESTORE, 
    SERVER_LOGIN_RESPONSE, SERVER_FILE_EXIST, SERVER_FILE_NON_EXIST};

#define CHUNK_QUEUE_SIZE 5000
#define CONTAINER_QUEUE_SIZE 100
#define REQUIRED_CONTAINER_CAPPING_VALUE 16

#endif