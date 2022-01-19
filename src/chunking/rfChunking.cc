/**
 * @file rfChunking.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface defined in Robin Fingerprint chunking algorithm 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/rfChunking.h"

struct timeval sTotalTime;
struct timeval eTotalTime;
struct timeval sTime;
struct timeval eTime;


/**
 * @brief Destroy the RFChunking object
 * 
 */
RFChunking::RFChunking(std::string path, int minSize, int maxSize, int avgSize) {
    this->LoadChunkFile(path);
    avgChunkSize_ = avgSize;
    minChunkSize_ = minSize;
    maxChunkSize_ = maxSize;
    int numOfMaskBits;

    // set the slidingWinSize 
    slidingWinSize_ = 256;
    // set the read buffer size
    readSize_ = 128 * 1024 * 1024;
    waitingForChunkingBuffer_ = new uint8_t[readSize_];
    chunkBuffer_ = new uint8_t[maxChunkSize_];

    // check memory allocation
    if ((!waitingForChunkingBuffer_) && (!chunkBuffer_)) {
        fprintf(stderr, "RFChunking: Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    if (minChunkSize_ >= avgChunkSize_) {
        fprintf(stderr, "RFChunking: minChunkSize should be smaller than avgChunkSize.\n");
        exit(EXIT_FAILURE);
    }

    if (maxChunkSize_ <= avgChunkSize_) {
        fprintf(stderr, "RFChunking: maxChunkSize should be larger than avgChunkSize.\n");
        exit(EXIT_FAILURE);
    }



    /*initialize the base and modulus for calculating the fingerprint of a window*/
    /*these two values were employed in open-vcdiff: "http://code.google.com/p/open-vcdiff/"*/
    polyBase_ = 257; /*a prime larger than 255, the max value of "unsigned char"*/
    polyMOD_ = (1 << 23) - 1; /*polyMOD - 1 = 0x7fffff: use the last 23 bits of a polynomial as its hash*/
    /*initialize the lookup table for accelerating the power calculation in rolling hash*/
    powerLUT_ = (uint32_t*)malloc(sizeof(uint32_t) * slidingWinSize_);
    /*powerLUT[i] = power(polyBase, i) mod polyMOD*/
    powerLUT_[0] = 1;
    for (int i = 1; i < slidingWinSize_; i++) {
        /*powerLUT[i] = (powerLUT[i-1] * polyBase) mod polyMOD*/
        powerLUT_[i] = (powerLUT_[i - 1] * polyBase_) & polyMOD_;
    }
    /*initialize the lookup table for accelerating the byte remove in rolling hash*/
    removeLUT_ = (uint32_t*)malloc(sizeof(uint32_t) * 256); /*256 for unsigned char*/
    for (int i = 0; i < 256; i++) {
        /*removeLUT[i] = (- i * powerLUT[_slidingWinSize-1]) mod polyMOD*/
        removeLUT_[i] = (i * powerLUT_[slidingWinSize_ - 1]) & polyMOD_;
        if (removeLUT_[i] != 0) {

            removeLUT_[i] = (polyMOD_ - removeLUT_[i] + 1) & polyMOD_;
        }
        /*note: % is a remainder (rather than modulus) operator*/
        /*      if a < 0,  -polyMOD < a % polyMOD <= 0       */
    }

    /*initialize the anchorMask for depolytermining an anchor*/
    /*note: power(2, numOfanchorMaskBits) = avgChunkSize*/
    numOfMaskBits = 1;
    while ((avgChunkSize_ >> numOfMaskBits) != 1) {
        numOfMaskBits++;
    }
    anchorMask_ = (1 << numOfMaskBits) - 1;
    /*initialize the value for depolytermining an anchor*/
    anchorValue_ = 0;
}
/**
 * @brief open the chunking file according to the path
 * 
 * @param path the path of the chunking file 
 */
void RFChunking::LoadChunkFile(std::string path) {
    if (chunkingFile_.is_open()) {
        chunkingFile_.close();
    }
    chunkingFile_.open(path, std::ios::binary);
    if (!chunkingFile_.is_open()) {
        fprintf(stderr, "RFChunking: open file %s error, exit now.\n", path.c_str());
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief implement the variable size chunking
 * 
 */
void RFChunking::VarSizeChunking() {
    uint16_t winFp;
    uint64_t chunkBufferCnt = 0, chunkIDCnt = 0;
    ifstream& fin = GetChunkingFile();
    uint64_t fileSize = 0;
    double totalTime = 0;

    // starting chunking
#if CHUNKING_BREAKDOWN == 1
    gettimeofday(&sTotalTime, NULL);
#endif 
    memset((char*)waitingForChunkingBuffer_, 0, sizeof(uint8_t) * readSize_);
    while (true) {
        fin.read((char*)waitingForChunkingBuffer_, sizeof(uint8_t) * readSize_);
        int len = fin.gcount();
        fileSize += len;
        for (size_t i = 0; i < len; i++) {
            chunkBuffer_[chunkBufferCnt] = waitingForChunkingBuffer_[i];

            // full fill sliding window
            if (chunkBufferCnt < slidingWinSize_) {
                //Refer to doc/Chunking.md hash function:RabinChunker
                winFp = winFp + (chunkBuffer_[chunkBufferCnt] * powerLUT_[slidingWinSize_ - chunkBufferCnt - 1]) & polyMOD_; 
                chunkBufferCnt++;
                continue;
            }
            winFp &= (polyMOD_);

            // slide window
            uint16_t v = chunkBuffer_[chunkBufferCnt - slidingWinSize_]; //queue
            //remove queue front and add queue tail
            winFp = ((winFp + removeLUT_[v]) * polyBase_ + chunkBuffer_[chunkBufferCnt]) & polyMOD_; 
            chunkBufferCnt++;

            /*chunk's size less than minChunkSize*/
            if (chunkBufferCnt < minChunkSize_)
                continue; 
                
            // find chunk pattern 
            if ((winFp & anchorMask_) == anchorValue_) {
                Data_t tempChunk;
                tempChunk.chunk.ID = chunkIDCnt;
                tempChunk.chunk.logicDataSize = chunkBufferCnt;
                memcpy(tempChunk.chunk.data, chunkBuffer_, chunkBufferCnt);
                tempChunk.dataType = DATA_TYPE_CHUNK;

                // reset
                chunkIDCnt++;
                chunkBufferCnt = 0;
                winFp = 0;
                continue;
            }

            // cap the maximum chunk size 
            if (chunkBufferCnt >= maxChunkSize_) {
                Data_t tempChunk;
                tempChunk.chunk.ID = chunkIDCnt;
                tempChunk.chunk.logicDataSize = chunkBufferCnt;
                memcpy(tempChunk.chunk.data, chunkBuffer_, chunkBufferCnt);
                tempChunk.dataType = DATA_TYPE_CHUNK;

                // reset 
                chunkIDCnt++;
                chunkBufferCnt = 0;
                winFp = 0;
                continue;
            }
        }

        if (fin.eof()) {
            break;
        }
    }

    // add the final chunk
    if (chunkBufferCnt != 0) {
        Data_t tempChunk;
        tempChunk.chunk.ID = chunkIDCnt;
        tempChunk.chunk.logicDataSize = chunkBufferCnt;
        memcpy(tempChunk.chunk.data, chunkBuffer_, chunkBufferCnt);
        tempChunk.chunk.type = DATA_TYPE_CHUNK;

        // reset 
        chunkIDCnt++;
        chunkBufferCnt = 0;
        winFp = 0;
    }
    fileRecipe_.recipe.fileRecipeHead.totalChunkNumber = chunkIDCnt;
    fileRecipe_.recipe.keyRecipeHead.totalChunkKeyNumber = chunkIDCnt;
    fileRecipe_.recipe.fileRecipeHead.fileSize = fileSize;
    fileRecipe_.recipe.keyRecipeHead.fileSize = fileSize;
    fileRecipe_.dataType = DATA_TYPE_RECIPE;
#if CHUNKING_BREAKDOWN == 1
    gettimeofday(&eTotalTime, NULL);
    totalTime = tool::GetTimeDiff(sTotalTime, eTotalTime);
    fprintf(stderr, "RFChunking: total chunking time = %.4lf\n", totalTime);
    fprintf(stderr, "Total chunk number: %lu\n", chunkIDCnt);
    fprintf(stderr, "Chunking file size (MB): %.4lf\n", (fileSize / 1024.0 / 1024.0));
    fprintf(stderr, "Throughput (MB/s): %.4lf\n", (fileSize / 1024.0 / 1024.0) / totalTime);
#endif
    return ;
}


/**
 * @brief Get the Chunking File object
 * 
 * @return std::ifstream& return the reference to the chunking file 
 */
std::ifstream& RFChunking::GetChunkingFile() {
    if (!chunkingFile_.is_open()) {
        fprintf(stderr, "RFChunking: chunking file open failed.\n");
        exit(EXIT_FAILURE);
    } 
    return chunkingFile_;
}


/**
 * @brief run the chunking process
 * 
 */
void RFChunking::Chunking() {
    this->VarSizeChunking();
}

/**
 * @brief Destroy the RFChunking object
 * 
 */
RFChunking::~RFChunking() {
    fprintf(stderr, "RFChunking: Destory the object.\n");
    
    if (waitingForChunkingBuffer_) {
        delete[] waitingForChunkingBuffer_;
    }

    if (chunkBuffer_) {
        delete[] chunkBuffer_;
    }

    if (powerLUT_) {
        free(powerLUT_);
    }

    if (removeLUT_) {
        free(removeLUT_);
    }
}