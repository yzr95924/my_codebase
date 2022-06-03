/**
 * @file fix_chunker.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface of FixChunker
 * @version 0.1
 * @date 2022-06-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/dedup/fix_chunker.h"

/**
 * @brief Construct a new Fix Chunker object
 * 
 */
FixChunker::FixChunker() {
    tool::Logging(my_name_.c_str(), "init FixChunker.\n");
}


/**
 * @brief load the data from the file
 * 
 * @param input_file the input file handler
 * @return uint32_t the read size
 */
uint32_t FixChunker::LoadDataFromFile(ifstream& input_file) {
    input_file.read((char*)read_data_buf_, read_size_);
    pending_chunking_size_ = input_file.gcount();
    return pending_chunking_size_;
}

/**
 * @brief generate a chunk
 * 
 * @param data the buffer to store the chunk data
 * @return uint32_t the chunk size
 */
uint32_t FixChunker::GenerateOneChunk(uint8_t* data) {
    if (pending_chunking_size_ == 0) {
        return 0; // this is end of the pending buffer
    }

    // TODO: fix here
    if (remain_chunking_size_ > )

}