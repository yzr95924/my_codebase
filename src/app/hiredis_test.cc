/**
 * @file hiredis_test.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief the main program to hiredis testing
 * @version 0.1
 * @date 2022-08-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <sw/redis++/redis++.h>
#include "../../include/define.h"

uint64_t TEST_TIME = 100000000;

using namespace std;

uint32_t RandomGen(uint32_t a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

int main(int argc, char* argv[]) {
    /* code */
    auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");
    std::cout << redis.ping() << std::endl;

    string test_key = "key";
    string test_val = "value";


    struct timeval s_time;
    struct timeval e_time;

    gettimeofday(&s_time, NULL);
    uint32_t random_data;
    for (size_t i = 0; i < TEST_TIME; i++) {
        uint32_t seed = RandomGen(i);
        // default_random_engine eng{i};
        // uniform_int_distribution<uint32_t> random_range(0, UINT32_MAX);
        // random_data = random_range(eng);
        random_data = RandomGen(seed);
    }
    gettimeofday(&e_time, NULL);

    double total_time = tool::GetTimeDiff(s_time, e_time);
    uint64_t total_size = TEST_TIME * sizeof(uint32_t);

    cout << "speed: " << static_cast<double>(total_size) / 1024.0 / 1024.0 / total_time << endl;

    redis.set(test_key, test_val);
    cout << "test set key: " << *(redis.get(test_key)) << endl;

    redis.del(test_key);
    cout << "test del key: " << (redis.exists(test_key)) << endl;

    redis.save();

    return 0;
}
