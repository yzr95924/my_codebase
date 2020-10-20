/**
 * @file compress.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface of compression 
 * @version 0.1
 * @date 2020-10-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MY_CODEBASE_COMPRESS_H
#define MY_CODEBASE_COMPRESS_H

#include "define.h"
#include <lz4.h>

using namespace std;

class Compresser {
    private:
        uint8_t* readBuffer_;
        uint8_t* outputBuffer_;
        uint32_t readSize_ = 16 * 1024 * 1024;

        ifstream inputData_;
        ofstream outputData_;

    public:
        /**
         * @brief Construct a new Compresser object
         * 
         * @param path the path of input file  
         */
        Compresser(string path);

        /**
         * @brief Destroy the Compresser object
         * 
         */
        ~Compresser();

        /**
         * @brief do the compression
         * 
         */
        void Compression();

        /**
         * @brief do the decompression 
         * 
         */
        void Decompression();

};

#endif // !MY_CODEBASE_COMPRESS_H