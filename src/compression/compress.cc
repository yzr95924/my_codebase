/**
 * @file compress.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface defined in the compressor 
 * @version 0.1
 * @date 2020-10-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/compress.h"

/**
 * @brief Construct a new Compresser:: Compresser object
 * 
 * @param path 
 */
Compresser::Compresser(string path) {
    string outputPath = path + "-tar";
    inputData_.open(path, std::ios_base::in | std::ios_base::binary);    
    if (!inputData_.is_open()) {
        fprintf(stderr, "Compresser: Cannot open the input file.\n");
        exit(EXIT_FAILURE);
    }

    outputData_.open(outputPath, std::ios_base::trunc | std::ios_base::binary);
    if (!outputData_.is_open()) {
        fprintf(stderr, "Conpresser: Cannot open the output file.\n");
        exit(EXIT_FAILURE);
    }

    readBuffer_ = (uint8_t*) malloc(sizeof(uint8_t) * readSize_);
    outputBuffer_ = (uint8_t*) malloc(sizeof(uint8_t) * readSize_ * 2);
    fprintf(stderr, "Compresser: Initialize the compresser.\n");
}


/**
 * @brief Destroy the Compresser object
 * 
 */
Compresser::~Compresser() {
    if (readBuffer_) {
        free(readBuffer_);
    }

    if (outputBuffer_) {
        free(outputBuffer_);
    }

    if (inputData_.is_open()) {
        inputData_.close();
    }

    if (outputData_.is_open()) {
        outputData_.close();
    }
    fprintf(stderr, "Compresser: Destory the compresser.\n");    
}



/**
 * @brief do the compression
 * 
 */
void Compresser::Compression() {
    struct timeval sTime, eTime;
    bool end = false;
    gettimeofday(&sTime, NULL);
    while (!end) {
        inputData_.read((char*)readBuffer_, sizeof(uint8_t) * readSize_);
        end = inputData_.eof();
        int len = inputData_.gcount();
        int compressedSize = LZ4_compress_default((const char*)readBuffer_, (char*)outputBuffer_, len,
            readSize_ * 2);
        fprintf(stderr, "Compresser: CompressedSize size: %d\n", compressedSize);
        outputData_.write((const char*)outputBuffer_, compressedSize);
    }
    gettimeofday(&eTime, NULL);
    double totalTime = tool::GetTimeDiff(sTime, eTime);
    fprintf(stderr, "Compresser: finish compression.\n");
    fprintf(stderr, "Compresser: total time: %.4lf\n", totalTime);
}


/**
 * @brief do the decompression 
 * 
 */
void Compresser::Decompression() {
    struct timeval sTime, eTime;
    bool end = false;
    gettimeofday(&sTime, NULL);
    while (!end) {
        inputData_.read((char*)readBuffer_, sizeof(uint8_t) * readSize_);
        end = inputData_.eof();
        int len = inputData_.gcount();
        int decompressedSize = LZ4_decompress_safe((const char*)readBuffer_, (char*)outputBuffer_, 
            len, readSize_ * 2);
        fprintf(stderr, "Compresser: Decompressed size: %d\n", decompressedSize);
        outputData_.write((const char*)outputBuffer_, decompressedSize);
    }
    gettimeofday(&eTime, NULL);
    double totalTime = tool::GetTimeDiff(sTime, eTime);
    fprintf(stderr, "Compresser: finish decompression.\n");
    fprintf(stderr, "Compresser: total time: %.4lf\n", totalTime);
}
