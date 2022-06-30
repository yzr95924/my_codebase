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

using namespace std;

int main(int argc, char const *argv[]) {
    uint32_t test_time = 100000;
    uint8_t max_padding[MAX_CHUNK_SIZE] = {0};

    struct timeval stime;
    struct timeval etime;
    double total_running_time = 0;
    uint64_t total_size = 0;

    gettimeofday(&stime, NULL);
    for (size_t i = 0; i < test_time; i++) {
        default_random_engine random_eng{i};
        uniform_int_distribution<uint64_t> data_gen(0, UINT64_MAX);
        uint64_t* data_ptr = (uint64_t*) max_padding;
        size_t block_num = (i % MAX_CHUNK_SIZE) / sizeof(uint64_t);
        for (size_t j = 0; j < block_num; j++) {
            *data_ptr = data_gen(random_eng);
            data_ptr++;
        }
        total_size += (i % MAX_CHUNK_SIZE);
    }
    gettimeofday(&etime, NULL);
    total_running_time += tool::GetTimeDiff(stime, etime); 

    fprintf(stderr, "throughput: %lfMiB/s\n",
        static_cast<double>(total_size / 1024.0 / 1024.0) / total_running_time);

    return 0;
}
