/**
 * @file fixChunking.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface defined in fixed-size chunking algorithm 
 * @version 0.1
 * @date 2020-09-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/fixChunking.h"

struct timeval startTime;
struct timeval endTime;
/**
 * @brief Construct a new Fix Chunking object
 * 
 * @param filePath 
 * @param minSize 
 * @param maxSize 
 */
FixChunking::FixChunking(string filePath, int minSize, int maxSize, int avgSize) {
    this->LoadChunkFile(filePath);

    minChunkSize_ = minSize;
    maxChunkSize_ = maxSize;
    avgChunkSize_ = avgSize;

    // readsize 128MB
    readSize_ = 128 * 1024 * 1024;
    waitingForChunkingBuffer_ = new uint8_t[readSize_];
    chunkBuffer_ = new uint8_t[avgChunkSize_];

    if ((!waitingForChunkingBuffer_) && (!chunkBuffer_)) {
        fprintf(stderr, "FixChunking: memory error.\n");
        exit(EXIT_FAILURE);
    }

    if (readSize_ % avgChunkSize_ != 0) {
        fprintf(stderr, "FixChunking: ReadSize is not the multiple of the avgChunkSize_.\n");
        exit(EXIT_FAILURE);
    }

}


/**
 * @brief load the chunking file 
 * 
 * @param path the path of the chunking file
 */
void FixChunking::LoadChunkFile(string filePath) {
    if (chunkingFile_.is_open()) {
        chunkingFile_.close();
    } 

    chunkingFile_.open(filePath, std::ios::binary);

    if (!chunkingFile_.is_open()) {
        fprintf(stderr, "FixChunking: open file: %s error.\n", filePath.c_str());
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Get the Chunking File object
 * 
 * @return std::ifstream& 
 */
std::ifstream& FixChunking::GetChunkingFile() {
    if (!chunkingFile_.is_open()) {
        fprintf(stderr, "FixChunking: chunking file open failed.\n");
        exit(EXIT_FAILURE);
    }
    return chunkingFile_;
}

/**
 * @brief Destroy the Fix Chunking object
 * 
 */
FixChunking::~FixChunking() {
    fprintf(stderr, "FixChunking: Destroy the object.\n");
    if (waitingForChunkingBuffer_) {
        delete[] waitingForChunkingBuffer_;
    } 

    if (chunkBuffer_) {
        delete[] chunkBuffer_;
    }
}

/**
 * @brief fixed-size chunking algorithm 
 * 
 */
void FixChunking::FixSizeChunking() {
    std::ifstream& fin = GetChunkingFile();
    uint64_t chunkIDCnt = 0;
    memset(chunkBuffer_, 0, sizeof(int8_t) * avgChunkSize_);
    uint64_t fileSize = 0;
    double totalTime = 0;

    // start chunking
#if CHUNKING_BREAKDOWN == 1
    gettimeofday(&startTime, NULL);
#endif

    memset(waitingForChunkingBuffer_, 0, sizeof(int8_t) * readSize_);
    while (true) {
        fin.read((char*) waitingForChunkingBuffer_, sizeof(uint8_t) * readSize_);
        uint64_t totalReadSize = fin.gcount();
        fileSize += totalReadSize;
        uint64_t chunkedSize = 0;
        if (totalReadSize == readSize_) {

            memset(chunkBuffer_, 0, sizeof(uint8_t) * avgChunkSize_);
            while (chunkedSize < totalReadSize) {
                memcpy(chunkBuffer_, waitingForChunkingBuffer_ + chunkedSize,
                    avgChunkSize_);
                Data_t tempChunk;
                tempChunk.chunk.chunkSize = avgChunkSize_;
                memcpy(tempChunk.chunk.logicData, chunkBuffer_, avgChunkSize_);
                tempChunk.dataType = DATA_CHUNK;

                chunkIDCnt++;
                chunkedSize += avgChunkSize_;
            }

        } else {
            uint64_t retSize = totalReadSize - chunkedSize;
            while (chunkedSize < totalReadSize) {
                Data_t tempChunk;
                if (retSize > avgChunkSize_) {
                    memcpy(chunkBuffer_, waitingForChunkingBuffer_ + chunkedSize, avgChunkSize_);

                    tempChunk.chunk.chunkSize = avgChunkSize_;
                    memcpy(tempChunk.chunk.logicData, chunkBuffer_, avgChunkSize_);
                    chunkedSize += avgChunkSize_;
                } else {
                    memcpy(chunkBuffer_, waitingForChunkingBuffer_ + chunkedSize, retSize);

                    tempChunk.chunk.chunkSize = retSize;
                    memcpy(tempChunk.chunk.logicData, chunkBuffer_, retSize);
                    chunkedSize += retSize;
                }
                tempChunk.dataType = DATA_CHUNK;
                retSize = totalReadSize - chunkedSize;
                chunkIDCnt++;
            }
        }

        if (fin.eof()) {
            break;
        }
    }
    fileRecipe_.recipeEnd.totalChunkNumber = chunkIDCnt;
    fileRecipe_.recipeEnd.fileSize = fileSize;
    fileRecipe_.dataType = RECIPE_END;

#if CHUNKING_BREAKDOWN == 1
    gettimeofday(&endTime, NULL);
    totalTime += tool::GetTimeDiff(startTime, endTime);
    fprintf(stderr, "FixChunking: total chunking time = %.4lf\n", totalTime);
    fprintf(stderr, "Total chunk number: %lu\n", chunkIDCnt);
    fprintf(stderr, "Chunking file size (MB): %.4lf\n", (fileSize / 1024.0 / 1024.0));
    fprintf(stderr, "Throughput (MB/s): %.4lf\n", (fileSize / 1024.0 / 1024.0) / totalTime);
#endif

}


/**
 * @brief run the chunking algorithm
 * 
 */
void FixChunking::Chunking() {
    this->FixSizeChunking();
}