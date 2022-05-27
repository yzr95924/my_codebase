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
    switch (compress_type) {
        case LZ4_COMPRESS_TYPE: {
            compress_type_ = LZ4_COMPRESS_TYPE;
            break;
        }
        case ZSTD_COMPRESS_TYPE: {
            compress_type_ = ZSTD_COMPRESS_TYPE;
            break;
        }
        case ZLIB_COMPRESS_TYPE: {
            compress_type_ = ZLIB_COMPRESS_TYPE;
            break;
        }
        default: {
            tool::Logging(my_name_.c_str(), "wrong compression type.\n");
            exit(EXIT_FAILURE);
        }
    }
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
    bool compress_flag = false;
    int compressed_size = 0;
    switch (compress_type_) {
        case LZ4_COMPRESS_TYPE: {
            compressed_size = LZ4_compress_fast((char*)chunk_data, (char*)output_data,
                chunk_size, MAX_CHUNK_SIZE, compress_level_);
            if (compressed_size <= 0) {
                // it cannot do compression
                memcpy(output_data, chunk_data, chunk_size);
                compress_flag = false;
            } else {
                // it can do compression
                *output_size = compressed_size;
                compress_flag = true;
            }
            break;
        }
        case ZSTD_COMPRESS_TYPE: {
            compressed_size = ZSTD_compress(output_data, MAX_CHUNK_SIZE, chunk_data, 
                chunk_size, compress_level_);
            if (ZSTD_isError(compressed_size)) {
                // it cannot do compression
                memcpy(output_data, chunk_data, chunk_size);
                compress_flag = false;
            } else {
                // it can do compression
                *output_size = compressed_size;
                compress_flag = true;
            }
            break;
        }
        case ZLIB_COMPRESS_TYPE: {
            break;
        }
    }
    return compress_flag; 
}

/**
 * @brief Destroy the Compress Util object
 * 
 */
CompressUtil::~CompressUtil() {
}