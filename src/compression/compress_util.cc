/**
 * @file compress_util.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface of the CompressUtil
 * @version 0.1
 * @date 2022-05-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/compression/compress_util.h"


/**
 * @brief Construct a new Compress Util object
 * 
 * @param compress_type the compression algo type
 * @param compress_level the compression level type 
 */
CompressUtil::CompressUtil(uint32_t compress_type, uint32_t compress_level) {
    compress_type_ = compress_type;
    compress_level_ = compress_level;
}

/**
 * @brief compress a chunk
 * 
 * @param chunk_data the chunk data
 * @param chunk_size the chunk size
 * @param output_data the chunk data
 * @param output_size the chunk size
 * @return true it can compress
 * @return false it cannot compress
 */
bool CompressUtil::CompressOneChunk(uint8_t* chunk_data, uint32_t chunk_size, 
    uint8_t* output_data, uint32_t* output_size) {

}

/**
 * @brief Destroy the Compress Util object
 * 
 */
CompressUtil::~CompressUtil() {

}