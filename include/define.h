/**
 * @file define.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief include the necessary header 
 * @version 0.1
 * @date 2020-09-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CHUNKING_LIB_DEFINE
#define CHUNKING_LIB_DEFINE

#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <memory>
#include <string>
#include <list>
#include <locale>

static const uint64_t GB_2_B = 1000 * 1000 * 1000;
static const uint64_t GiB_2_B = uint64_t(1) << 30;
static const uint64_t MB_2_B = 1000 * 1000;
static const uint64_t MiB_2_B = uint64_t(1) << 20;
static const uint64_t KB_2_B = 1000;
static const uint64_t KiB_2_B = uint64_t(1) << 10;
static const uint64_t SEC_2_US = 1000 * 1000 * 1000;

namespace tool {

/**
 * @brief Get the Time Diff object
 * 
 * @param start_time start time
 * @param end_time end time
 * @return double the diff time (sec)
 */
double GetTimeDiff(struct timeval start_time, struct timeval end_time) {
    double second;
    second = (end_time.tv_sec - start_time.tv_sec) * static_cast<double>(SEC_2_US) + 
        end_time.tv_usec - start_time.tv_usec;
    second = second / static_cast<double>(SEC_2_US);
    return second;    
}

/**
 * @brief compare the limits with the input
 * 
 * @param input the input number
 * @param lower 
 * @param upper 
 * @return uint32_t 
 */
uint32_t CompareLimit(uint32_t input, uint32_t lower, uint32_t upper) {
    if (input <= lower) {
        return lower; 
    } else if (input >= upper) {
        return upper;
    } else {
        return input;
    }    
}

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return uint32_t 
 */
uint32_t DivCeil(uint32_t a, uint32_t b) {

}

/**
 * @brief 
 * 
 * @param fp 
 * @param fp_size 
 */
void PrintBinaryArray(const uint8_t* fp, size_t fp_size) {

}

void Logging(std::string const& msg) {
    time_t t = std::time(nullptr);
    std::locale loc;
    const std::time_put<char>& tp = std::use_facet<std::time_put<char>>(loc);
    const char* fmt = "%F %T";
    tp.put(std::cout, std::cout, ' ', std::localtime(&t), fmt, fmt + strlen(fmt));
    std::cout << ": " << msg << std::endl;    
}

} // namespace tool
#endif 