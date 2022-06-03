/**
 * @file data_structure.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief the basic data structure
 * @version 0.1
 * @date 2022-06-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_CODEBASE_DATA_STRUCTURE_H 
#define MY_CODEBASE_DATA_STRUCTURE_H

#include "constVar.h"
#include <bits/stdc++.h>

typedef struct {
    uint32_t size;
    uint8_t data[MAX_CHUNK_SIZE];
    uint8_t fp[CHUNK_HASH_SIZE];
    uint8_t key[CHUNK_HASH_SIZE];
    uint8_t stat;
} Chunk_t;

typedef struct {
    uint64_t size;
    uint64_t chunk_num;
} FileRecipeHead_t;

typedef struct {
    uint8_t hash[CHUNK_HASH_SIZE];
    uint8_t key[CHUNK_HASH_SIZE];
} RecipeEntry_t;

typedef struct {
    union {
        Chunk_t chunk;
        FileRecipeHead_t head;
    };
    int type;
} Data_t;

typedef struct {
    uint8_t hash[CHUNK_HASH_SIZE];
    uint64_t sf_1;
    uint64_t sf_2;
    uint64_t sf_3;
} CacheMeta_t;

typedef struct {
    uint8_t stat; // simlar or non-similar
    uint8_t comp_flag; // compressed or uncompressed 
    uint8_t container_id[CONTAINER_ID_LENGTH];
    uint32_t offset;
    uint32_t len;
    uint8_t base_chunk_hash[CHUNK_HASH_SIZE];
} KeyForChunkHashDB_t;

typedef struct {
    int size;
    uint8_t data[MAX_CHUNK_SIZE];
} RetrieverData_t;

typedef struct {
    char id[CONTAINER_ID_LENGTH];
    uint8_t body[MAX_CONTAINER_SIZE];
    uint32_t cur_size;
} Container_t;

typedef struct {
    uint32_t client_id;
    uint32_t msg_type;
    uint32_t cur_item_num;
    uint32_t size;
} NetworkHead_t;

typedef struct {
    NetworkHead_t* header;
    uint8_t* send_buf;
    uint8_t* data_buf;
} SendMsgBuffer_t;

typedef struct {
    uint8_t fp[CHUNK_HASH_SIZE]; // chunk fp
    KeyForChunkHashDB_t addr;
} ChunkStatus_t;

typedef struct {
    ChunkStatus_t stat;
    uint8_t data[MAX_CHUNK_SIZE];
} WrappedChunk_t;

typedef struct {
    uint8_t data[MAX_CHUNK_SIZE];
    uint32_t size;
} RestoreChunk_t;

#endif