/**
 * @file dataStructure.h
 * @author Zhao Jia
 * @brief define the data structure in deduplication
 * @version 0.1
 * @date 2021-8
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include "constVar.h"
#include <bits/stdc++.h>

typedef struct {
    uint32_t chunkSize; // for SIM chunk: the original length of chunk before encoding
    uint8_t logicData[MAX_CHUNK_SIZE];
    uint8_t chunkHash[CHUNK_HASH_SIZE];
    uint8_t key[CHUNK_HASH_SIZE];
} Chunk_t;

typedef struct {
    uint64_t fileSize;
    uint64_t totalChunkNumber;
} FileRecipeEnd_t;

typedef struct {
    uint8_t cipherChunkHash[CHUNK_HASH_SIZE];
    uint8_t key[CHUNK_HASH_SIZE];
} RecipeEntry_t;

typedef struct {
    union {
        Chunk_t chunk;
        FileRecipeEnd_t recipeEnd;
    };
    int dataType;
} Data_t;

typedef struct {
    uint8_t chunkStatus; // simlar or non-similar
    uint8_t compressFlag; // compressed or uncompressed 
    uint8_t containerName[CONTAINER_ID_LENGTH];
    uint32_t offset;
    uint32_t length;
    uint8_t basechunkHash[CHUNK_HASH_SIZE];
} KeyForChunkHashDB_t;

typedef struct {
    int logicDataSize;
    uint8_t logicData[MAX_CHUNK_SIZE];
} RetrieverData_t;

typedef struct {
    char containerID[CONTAINER_ID_LENGTH];
    uint8_t body[MAX_CONTAINER_SIZE];
    uint32_t currentSize;
} Container_t;

typedef struct {
    uint32_t clientID;
    uint32_t messageType;
    uint32_t currentItemNum;
    uint32_t dataSize;
} NetworkHead_t;

typedef struct {
    NetworkHead_t* header;
    uint8_t* sendBuffer;
    uint8_t* dataBuffer;
} SendMsgBuffer_t;

typedef struct {
    uint8_t fp[CHUNK_HASH_SIZE]; // chunk fp
    KeyForChunkHashDB_t addr;
} ChunkStatus_t;

typedef struct {
    ChunkStatus_t chunkStatus;
    uint8_t data[MAX_CHUNK_SIZE];
} WrappedChunk_t;

typedef struct {
    uint8_t data[MAX_CHUNK_SIZE];
    uint32_t chunkSize;
} RestoreChunk_t;

#endif