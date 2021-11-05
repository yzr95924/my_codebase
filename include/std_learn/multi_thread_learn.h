/**
 * @file multThreadLearning.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief the lib for std multi-threading learn
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MULTI_THREAD_LEARNING_H
#define MULTI_THREAD_LEARNING_H

#include "define.h"
#include <future>
#include <chrono>

using namespace std;

class MultiThreadLearning {
    private:
        string class_name_ = "MultiThreadLearning";
        
        /**
         * @brief print the future when it received 
         * 
         * @param fut_int the future var to store the shared state
         */
        void PrintFutureInt(future<int>& fut_int);

        /**
         * @brief a non-optimized way of checking for prime numbers
         * 
         * @param x 
         * @return true 
         * @return false 
         */
        bool IsPrime(int x);

    public:
        /**
         * @brief Construct a new Multi Thread Learning object
         * 
         */
        MultiThreadLearning() {
            tool::Logging(class_name_.c_str(), "Init MultiThreadLearning.\n");
        }

        ~MultiThreadLearning() {
            tool::Logging(class_name_.c_str(), "Destory MultiThreadLearning.\n");
        }

        /**
         * @brief test with the promise
         * 
         */
        void PromiseTest();

        /**
         * @brief test with the future
         * 
         */
        void FutureTest();
};



#endif