/**
 * @file tmp_main.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief a tmp main file
 * @version 0.1
 * @date 2021-10-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../../include/std_learn/lambda_learn.h"
#include "../../include/std_learn/multi_thread_learn.h"

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static uint64_t s[2] = {1};

uint64_t next(void) {
	const uint64_t s0 = s[0];
	uint64_t s1 = s[1];
	const uint64_t result = s0 + s1;

	s1 ^= s0;
	s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
	s[1] = rotl(s1, 37); // c

	return result;
}

int main(int argc, char* argv[]) {
    next();
    tool::PrintBinaryArray((uint8_t*)s, sizeof(uint64_t) * 2);
    next();
    tool::PrintBinaryArray((uint8_t*)s, sizeof(uint64_t) * 2);
    next();
    tool::PrintBinaryArray((uint8_t*)s, sizeof(uint64_t) * 2);
    return 0;
}
