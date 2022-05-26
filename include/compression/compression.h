/**
 * @file compression.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface for local compression
 * @version 0.1
 * @date 2022-05-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_CODEBASE_COMPRESS_H
#define MY_CODEBASE_COMPRESS_H

#include "../constVar.h"
#include "../define.h"

// for compression header 
#include <lz4.h>
#include <zstd.h>
#include <zlib.h>

using namespace std;

class CompressUtil {
    private:
        string my_name_ = "CompressUtil";
        uint32_t compress_type_ = 0;
        uint32_t compress_level_ = 0;

    public:

        /**
         * @brief Construct a new Compress Util object
         * 
         * @param compress_type 
         * @param compress_level 
         */
        CompressUtil(uint32_t compress_type, uint32_t compress_level);

        /**
         * @brief Destroy the Compress Util object
         * 
         */
        ~CompressUtil();

};

#endif