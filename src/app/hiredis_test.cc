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

#include <hiredis/hiredis.h>
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
    // auto redis = sw::redis::Redis("tcp://127.0.0.1:6379/1"); // choose the 1st database
    // std::cout << redis.ping() << std::endl;

    redisContext* redis_ctx;
    redisReply* redis_reply;

    redis_ctx = redisConnect("127.0.0.1", 6379);
    if (redis_ctx->err) {
        cout << "error: " << redis_ctx->errstr << endl;
        exit(EXIT_FAILURE);
    }

    redis_reply = (redisReply*)redisCommand(redis_ctx, "SELECT 1");
    freeReplyObject(redis_reply);
    redis_reply = (redisReply*)redisCommand(redis_ctx, "DBSIZE");
    if (redis_reply->type == REDIS_REPLY_INTEGER && redis_reply->integer == 0) {
        cout << "DB 1 is empty" << endl;
    } else {
        cout << "DB 1 is not empty" << endl;
    }
    freeReplyObject(redis_reply);

    // insert key-value pair
    uint8_t test_buf[16] = {1};
    tool::PrintBinaryArray(test_buf, 16);
    uint8_t key[16] = {0};

    redis_reply = (redisReply*)redisCommand(redis_ctx, "SET %b %b",
        key, 16, test_buf, 16);
    freeReplyObject(redis_reply);

    redis_reply = (redisReply*)redisCommand(redis_ctx, "GET %b",
        key, 16);
    cout << "Get" << endl;
    tool::PrintBinaryArray((uint8_t*)redis_reply->str, redis_reply->len);
    freeReplyObject(redis_reply);

    // clean the database
    redis_reply = (redisReply*)redisCommand(redis_ctx, "FLUSHDB");
    freeReplyObject(redis_reply);

    redisFree(redis_ctx);


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

    return 0;
}