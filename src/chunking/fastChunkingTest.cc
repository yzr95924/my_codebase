/**
 * @file fastChunkingTest.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface defined in the FastCDC.h 
 * @version 0.1
 * @date 2020-09-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#include "../../include/fastChunkingTest.h"


/**
 * @brief Construct a new Fast C D C object
 * 
 * @param filePath 
 * @param minSize 
 * @param maxSize 
 * @param avgSize 
 */
FastCDC::FastCDC(string filePath, int minSize, int maxSize, int avgSize) {
    this->minChunkSize_ = tool::CompareLimit(minSize, MINIMUM_MIN, MINIMUM_MAX);
    this->avgChunkSize_ = tool::CompareLimit(avgSize, AVERAGE_MIN, AVERAGE_MAX);
    this->maxChunkSize_ = tool::CompareLimit(maxSize, MAXIMUM_MIN, MAXIMUM_MAX);
    this->pos_ = 0;
    this->normalSize_ = CalNormalSize(minSize, avgSize, maxSize);

    uint32_t bits = (uint32_t) round(log2(static_cast<double>(avgSize))); 
    this->maskS_ = GenerateFastCDCMask(bits + 1);
    this->maskL_ = GenerateFastCDCMask(bits + 1);

    fprintf(stderr, "minChunkSize_: %u\n", this->minChunkSize_);
    fprintf(stderr, "avgChunkSize_: %u\n", this->avgChunkSize_);
    fprintf(stderr, "maxChunkSize_: %u\n", this->maxChunkSize_);
    fprintf(stderr, "normalSize_: %u\n", this->normalSize_);

}

/**
 * @brief generate the mask according to the given bits
 * 
 * @param bits the number of '1' + 1
 * @return uint32_t the returned mask
 */
uint32_t FastCDC::GenerateFastCDCMask(uint32_t bits) {
    uint32_t tmp;
    tmp = (1 << tool::CompareLimit(bits, 1, 31)) - 1;
    return tmp;
}

/**
 * @brief compute the normal size 
 * 
 * @param min 
 * @param max 
 * @param len 
 * @return uint32_t 
 */
uint32_t FastCDC::CalNormalSize(const uint32_t min, const uint32_t av,
    const uint32_t max) {
    uint32_t off = min + tool::DivCeil(min, 2);
    if (off > av) {
        off = av;
    } 
    uint32_t diff = av - off;
    if (diff > max) {
        return max;
    }
    return diff;
}
