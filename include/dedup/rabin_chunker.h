/**
 * @file rabin_chunker.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface of rabin fingerprint chunking
 * @version 0.1
 * @date 2022-06-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MY_CODEBASE_RABIN_CHUNKER_H
#define MY_CODEBASE_RABIN_CHUNKER_H

#include "abs_chunker.h"
#include "rabin_poly.h"
#include "new_rabin_poly.h"

class RabinChunker : public AbsChunker {
    protected:
        string my_name_ = "RabinChunker";

        // for compute rabin fingerprint
        RabinFPUtil* rabin_util_;
        RabinCtx_t rabin_ctx_;
        uint64_t mask_;

    public:
        /**
         * @brief Construct a new Rabin Chunker object
         * 
         */
        RabinChunker();

        /**
         * @brief Destroy the Rabin Chunker object
         * 
         */
        ~RabinChunker();

        /**
         * @brief load the data from the file
         * 
         * @param input_file the input file handler
         * @return uint32_t the read size
         */
        uint32_t LoadDataFromFile(ifstream& input_file);

        /**
         * @brief generate a chunk
         * 
         * @param data the buffer to store the chunk data
         * @return uint32_t the chunk size
         */
        uint32_t GenerateOneChunk(uint8_t* data);
};

#endif