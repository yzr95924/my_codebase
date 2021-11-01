/**
 * @file lambda_learn.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement lambda_learn
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../../include/std_learn/lambda_learn.h"


/**
 * @brief example to catch the outside variable
 * 
 */
void LambdaLearn::CaptureOutVar() {
    int a = 123;

    // basic: capture the value
    auto func_1 = [a, this]{ 
        tool::Logging(this->GetClassName().c_str(), 
            "basic: %d\n", a); 
    };
    func_1();

    // with parameter
    auto func_2 = [this](int input_var) {tool::Logging(this->GetClassName().c_str(),
        "with parameter: %d\n", input_var);};
    func_2(a);

    // capture the reference
    auto func_3 = [&a, this] {
        tool::Logging(this->GetClassName().c_str(), 
        "capture the reference: %d\n", a);
    };
    func_3();
    a = 321;
    func_3();

    // implicitly capture 
    auto func_4 = [&] {tool::Logging(this->GetClassName().c_str(),
        "implicitly capture (reference): %d\n", a); a = 100;};
    func_4();
    auto func_5 = [=] {tool::Logging(this->GetClassName().c_str(),
        "implicitly capture (value): %d\n", a);};
    func_5();

    return ;
}