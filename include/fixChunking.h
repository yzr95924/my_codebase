/**
 * @file fixChunking.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface of fixed-size chunking 
 * @version 0.1
 * @date 2020-09-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CHUNKING_LIB_FIX
#define CHUNKING_LIB_FIX

#include "define.h"
#include "dataStructure.h"

using namespace std;

class FixChunking {
private:
    int avgChunkSize_;
    int minChunkSize_;
    int maxChunkSize_;

    uint64_t readSize_;

    uint8_t* waitingForChunkingBuffer_;
    uint8_t* chunkBuffer_;

    std::ifstream chunkingFile_;
    uint64_t totalSize_;
    Data_t fileRecipe_;

    /**
     * @brief fixed-size chunking algorithm 
     * 
     */
    void FixSizeChunking();

    /**
     * @brief Get the Chunking File object
     * 
     * @return std::ifstream& 
     */
    std::ifstream& GetChunkingFile();

    /**
     * @brief load the chunking file 
     * 
     * @param path 
     */
    void LoadChunkFile(std::string path);
public:

    /**
     * @brief Construct a new Fix Chunking object
     * 
     * @param filePath 
     * @param minSize 
     * @param maxSize 
     */
    FixChunking(string filePath, int minSize, int maxSize, int avgSize);

    /**
     * @brief Destroy the Fix Chunking object
     * 
     */
    ~FixChunking();

    /**
     * @brief run the chunking algorithm
     * 
     */
    void Chunking();

};


#endif // !CHUNKING_LIB_FIX