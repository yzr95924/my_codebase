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

// namespace tool {
double tool::GetTimeDiff(struct timeval startTime, struct timeval endTime) {
    double second;
    second = (endTime.tv_sec - startTime.tv_sec) * 1000000.0 + endTime.tv_usec - startTime.tv_usec;
    second = second / 1000000.0;
    return second;
}
// }