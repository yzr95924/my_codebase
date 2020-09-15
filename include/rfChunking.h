/**
 * @file rfChunking.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface of robin fingerprint algorithm 
 * @version 0.1
 * @date 2020-09-07
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CHUNKING_LIB_RF
#define CHUNKING_LIB_RF

#include "define.h"
#include "dataStructure.h"


using namespace std;

class RFChunking {
private:
    // chunk size setting
    int avgChunkSize_;
    int minChunkSize_;
    int maxChunkSize_;

    // buffer
    uint8_t* waitingForChunkingBuffer_;
    uint8_t* chunkBuffer_;
    uint64_t readSize_;
    uint64_t totalSize_;
    Data_t fileRecipe_;
    std::ifstream chunkingFile_;

    // variable size chunking
    int slidingWinSize_;
    uint32_t polyBase_;
    // the modulus for limiting the value of the polynomial in rolling hash
    uint32_t polyMOD_;
    /*note: to avoid overflow, _polyMOD*255 should be in the range of "uint32_t"*/
    /*      here, 255 is the max value of "unsigned char"                       */
    /*the lookup table for accelerating the power calculation in rolling hash*/
    uint32_t* powerLUT_;
    /*the lookup table for accelerating the byte remove in rolling hash*/
    uint32_t* removeLUT_;
    /*the mask for determining an anchor*/
    uint32_t anchorMask_;
    /*the value for determining an anchor*/
    uint32_t anchorValue_;

    /**
     * @brief implement the variable size chunking
     * 
     */
    void VarSizeChunking();
    /**
     * @brief Get the Chunking File object
     * 
     * @return std::ifstream& return the reference to the chunking file 
     */
    std::ifstream& GetChunkingFile();

    /**
     * @brief open the chunking file according to the path
     * 
     * @param path the path of the chunking file 
     */
    void LoadChunkFile(std::string path);

public:
    /**
     * @brief Destroy the RFChunking object
     * 
     */
    RFChunking(std::string path, int minSize, int maxSize, int avgSize);
    /**
     * @brief Destroy the RFChunking object
     * 
     */
    ~RFChunking();

    /**
     * @brief run the chunking process
     * 
     */
    void Chunking();
};

#endif // !CHUNKING_LIB_RF