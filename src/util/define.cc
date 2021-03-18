/**
 * @file define.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement some common function in define.h 
 * @version 0.1
 * @date 2020-09-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/define.h"

double tool::GetTimeDiff(struct timeval startTime, struct timeval endTime) {
    double second;
    second = (endTime.tv_sec - startTime.tv_sec) * 1000000.0 + endTime.tv_usec - startTime.tv_usec;
    second = second / 1000000.0;
    return second;
}

uint32_t tool::CompareLimit(uint32_t input, uint32_t lower, uint32_t upper) {
    if (input <= lower) {
        return lower; 
    } else if (input >= upper) {
        return upper;
    } else {
        return input;
    }
}


uint32_t tool::DivCeil(uint32_t a, uint32_t b) {
    uint32_t tmp = a / b;
    if (a % b == 0) {
        return tmp;
    } else {
        return (tmp + 1);
    }
}

void tool::PrintBinaryArray(const uint8_t* fp, size_t fpSize) {
    for (size_t i = 0; i < fpSize; i++) {
        fprintf(stdout, "%02x", fp[i]);
    }
    fprintf(stdout, "\n");
}
