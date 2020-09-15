/**
 * @file dataStructure.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the data structure 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CHUNKING_LIB_DATASTRUCTURE
#define CHUNKING_LIB_DATASTRUCTURE

#include "define.h"
#include "configure.h"

using namespace std;



typedef struct {
    uint32_t ID;
    int type;
    int logicDataSize;
    uint8_t data[MAX_CHUNK_SIZE];
    uint8_t chunkHash[CHUNK_HASH_SIZE];
    uint8_t key[CHUNK_ENCRYPT_KEY_SIZE];
} Chunk_t;

typedef struct {
    uint64_t fileSize;
    uint8_t fileNameHash[FILE_NAME_HASH_SIZE];
    uint64_t totalChunkNumber;
} FileRecipeHead_t;


typedef struct {
    uint64_t fileSize;
    uint8_t fileNameHash[FILE_NAME_HASH_SIZE];
    uint64_t totalChunkKeyNumber;
} KeyRecipeHead_t;

typedef struct {
    FileRecipeHead_t fileRecipeHead;
    KeyRecipeHead_t keyRecipeHead;
} Recipe_t;


typedef struct {
    union {
        Chunk_t chunk;
        Recipe_t recipe;
    };
    int dataType;
} Data_t;

#endif // !CHUNKING_LIB_DATASTRUCTURE
