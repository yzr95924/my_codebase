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

#include <lz4.h>
#include <zstd.h>

using namespace std;

class CompressUtil {
    private:
        string myName_ = "CompressUtil";

    public:


};

#endif