/**
 * @file rabin_chunker.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface of RabinChunker
 * @version 0.1
 * @date 2022-06-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/dedup/rabin_chunker.h"

uint64_t GetBitNum(uint64_t input_num) {
    uint64_t tmp = input_num;
    uint64_t num_of_bit = 0;
    while (tmp / 2) {
        num_of_bit++;
        tmp /= 2; 
    }
    return num_of_bit;
}

/**
 * @brief Construct a new Rabin Chunker object
 * 
 */
RabinChunker::RabinChunker() {
    rabin_util_ = new RabinFPUtil(config.GetChunkerSlidingWinSize());
    rabin_util_->NewCtx(rabin_ctx_);
    uint64_t bit_of_avg = GetBitNum(avg_chunk_size_);
    mask_ = (1 << bit_of_avg) - 1;
}

/**
 * @brief Destroy the Rabin Chunker object
 * 
 */
RabinChunker::~RabinChunker() {
    rabin_util_->FreeCtx(rabin_ctx_);
    delete rabin_util_;
}

/**
 * @brief load the data from the file
 * 
 * @param input_file the input file handler
 * @return uint32_t the read size
 */
uint32_t RabinChunker::LoadDataFromFile(ifstream& input_file) {
    input_file.read((char*)read_data_buf_, read_size_);
    pending_chunking_size_ = input_file.gcount();

    // reset the offset
    cur_offset_ = 0;
    remain_chunking_size_ = pending_chunking_size_;

    return pending_chunking_size_;
}

/**
 * @brief generate a chunk
 * 
 * @param data the buffer to store the chunk data
 * @return uint32_t the chunk size
 */
uint32_t RabinChunker::GenerateOneChunk(uint8_t* data) {
    if (pending_chunking_size_ == 0 || remain_chunking_size_ == 0) {
        return 0; // this is end of the pending buffer
    }

    uint32_t chunk_size = 0;
    if (remain_chunking_size_ <= min_chunk_size_) {
        memcpy(data, read_data_buf_ + cur_offset_, remain_chunking_size_);
        chunk_size = remain_chunking_size_;
    } else {
        uint8_t* input_byte; 
        uint64_t cur_fp;
        uint64_t cur_pos = 0;

        while (true) {
            // find the chunk
            input_byte = read_data_buf_ + cur_offset_ + cur_pos;
            cur_fp = rabin_util_->SlideOneByte(rabin_ctx_, *input_byte);
            
            chunk_size++;
            cur_pos++;
            
            if ((chunk_size >= min_chunk_size_ && (cur_fp & mask_) == 0) ||
                chunk_size == max_chunk_size_ || (remain_chunking_size_ - chunk_size == 0)) {
                memcpy(data, read_data_buf_ + cur_offset_, chunk_size);
                rabin_util_->ResetCtx(rabin_ctx_);
                break;
            }
        }
    }

    cur_offset_ += chunk_size;
    remain_chunking_size_ -= chunk_size;

    _total_chunk_num++;
    _total_file_size += chunk_size;
    return chunk_size;
}