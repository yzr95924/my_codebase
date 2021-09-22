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
#ifndef MY_DEFINE_H
#define MY_DEFINE_H

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
    double GetTimeDiff(struct timeval start_time, struct timeval end_time);
    
    /**
     * @brief compare the limits with the input
     * 
     * @param input the input number
     * @param lower the lower bound of the limitation
     * @param upper the upper bound of the limitation
     * @return uint32_t 
     */
    uint32_t CompareLimit(uint32_t input, uint32_t lower, uint32_t upper);
    
    /**
     * @brief get the ceil of the division
     * 
     * @param a 
     * @param b 
     * @return uint32_t 
     */
    uint32_t DivCeil(uint32_t a, uint32_t b);
    
    /**
     * @brief print the binary buffer
     * 
     * @param fp the pointer to the buffer
     * @param fp_size the size of the buffer
     */
    void PrintBinaryArray(const uint8_t* buffer, size_t buffer_size);
    
    /**
     * @brief a simple logger
     * 
     * @param logger the logger name
     * @param fmt the input message
     */
    void Logging(const char* logger, const char* fmt, ...);

} // namespace tool
#endif 