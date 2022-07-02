/**
 * @file main_test.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief a simple test main program
 * @version 0.1
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/define.h"
#include "../../include/constVar.h"
#include "../../include/compression/compress_util.h"
#include "../../include/dedup/fastcdc_chunker.h"

using namespace std;

Configure config("config.json");

typedef struct {
    uint8_t raw_chunk[MAX_CHUNK_SIZE];
    uint32_t raw_size;
    uint8_t enc_chunk[MAX_CHUNK_SIZE];
    uint32_t enc_size;
    uint8_t key[CHUNK_HASH_SIZE];
    uint64_t seed;
} EncFeatureChunk_t;

CompressUtil* compress_util;
AbsChunker* test_chunker;
const uint32_t max_padding_size = 255;
uint8_t tmp_comp_data[MAX_CHUNK_SIZE] = {0};
string my_name = "MainTest";

void CompressionWithPadding(EncFeatureChunk_t* input_chunk) {
    // init the random eng with the seed
    default_random_engine random_eng{input_chunk->seed};
    uniform_int_distribution<uint64_t> data_gen(0, UINT64_MAX);

    // compute the padding size
    uint32_t padding_size = (input_chunk->seed % max_padding_size);
    uint32_t padding_block_num = padding_size / sizeof(uint64_t);
    padding_size = padding_block_num * sizeof(uint64_t);

    bool comp_flag = false;
    uint32_t tmp_comp_size = 0;
    comp_flag = compress_util->CompressOneChunk(
        input_chunk->raw_chunk, input_chunk->raw_size,
        tmp_comp_data, &tmp_comp_size
    );

    if (!comp_flag) {
        // it can directly return
    } else {
        // it can be compressed --> tmp_comp_chunk, check padding
        uint32_t size_after_padding = 0;
        if (tmp_comp_size + padding_size < input_chunk->raw_size) {
            uint64_t* padding_data = (uint64_t*) (tmp_comp_data + tmp_comp_size);
            for (size_t i = 0; i < padding_block_num; i++) {
                *padding_data = data_gen(random_eng);
                padding_data++;
            }
        }
    }

    return ;
}

int main(int argc, char const *argv[]) {
    EncFeatureChunk_t tmp_chunk;
    memset(tmp_chunk.key, 1, CHUNK_HASH_SIZE);

    string input_path;
    input_path.assign(argv[1]);
    ifstream input_file_hdl;
    input_file_hdl.open(input_path, ios_base::in | ios_base::binary);
    if (!input_file_hdl.is_open()) {
        tool::Logging(my_name.c_str(), "cannot open %s.\n", input_path.c_str());
        exit(EXIT_FAILURE);
    }

    struct timeval stime;
    struct timeval etime;
    struct timeval s_comp_pad_time;
    struct timeval e_comp_pad_time;
    double total_compress_padding_time = 0;
    double total_running_time = 0;
    uint64_t total_size = 0;

    gettimeofday(&stime, NULL);
    test_chunker = new FastCDC();
    compress_util = new CompressUtil(ZSTD_COMPRESS_TYPE, 1);

    bool is_end = false;
    while (!is_end) {
        uint64_t pending_size = 0;
        pending_size = test_chunker->LoadDataFromFile(input_file_hdl);
        if (pending_size == 0) {
            break;
        }
        total_size += pending_size;

        while (true) {
            tmp_chunk.raw_size = test_chunker->GenerateOneChunk(tmp_chunk.raw_chunk);
            
            if (!tmp_chunk.raw_size) {
                break;
            }

            tmp_chunk.seed = tmp_chunk.raw_size; 
            gettimeofday(&s_comp_pad_time, NULL);
            CompressionWithPadding(&tmp_chunk);
            gettimeofday(&e_comp_pad_time, NULL);
            total_compress_padding_time += tool::GetTimeDiff(s_comp_pad_time, e_comp_pad_time);
        }

    }

    gettimeofday(&etime, NULL);
    total_running_time += tool::GetTimeDiff(stime, etime); 

    fprintf(stderr, "throughput: %lfMiB/s\n",
        static_cast<double>(total_size / 1024.0 / 1024.0) / total_compress_padding_time);

    delete test_chunker;
    delete compress_util;
    input_file_hdl.close();
    return 0;
}
