/**
 * @file lambda_learn.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief learn std lambda function
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MY_CODE_BASE_LAMBDA_LEARN_H
#define MY_CODE_BASE_LAMBDA_LEARN_H

#include "../define.h"

using namespace std;

class LambdaLearn {
    private: 
        string class_name_ = "LambdaLearn";
    public: 
        /**
         * @brief Construct a new Lambda Learn object
         * 
         */
        LambdaLearn() {
            tool::Logging(class_name_.c_str(), "Init LambdaLearn.\n");
        }

        /**
         * @brief Destroy the Lambda Learn object
         * 
         */
        ~LambdaLearn() {
            tool::Logging(class_name_.c_str(), "Destroy LambdaLearn.\n");
        }

        /**
         * @brief example to catch the outside variable
         * 
         */
        void CaptureOutVar();

        inline string GetClassName() {
            return this->class_name_;
        }
};


#endif