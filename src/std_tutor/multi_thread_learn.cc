/**
 * @file multi_thread_learn.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief imple the multi thread learn
 * @version 0.1
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../../include/std_learn/multi_thread_learn.h"

/**
 * @brief print the future when it received 
 * 
 * @param fut_int the future var to store the shared state
 */
void MultiThreadLearning::PrintFutureInt(future<int>& fut_int) {
    tool::Logging(class_name_.c_str(), "Start the thread.\n");
    int received_val = fut_int.get();
    tool::Logging(class_name_.c_str(), "Received value: %d\n", received_val);
    return ;
}

/**
 * @brief a non-optimized way of checking for prime numbers
 * 
 * @param x 
 * @return true 
 * @return false 
 */
bool MultiThreadLearning::IsPrime(int x) {
    tool::Logging(class_name_.c_str(), "Start the thread.\n");
    for (int i = 2; i < x; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}


/**
 * @brief test with te
 * 
 */
void MultiThreadLearning::PromiseTest() {
    using namespace std;
    promise<int> prom_obj; // create promise
    future<int> fut_int = prom_obj.get_future(); // get the share state
    thread th_1(&MultiThreadLearning::PrintFutureInt, this, ref(fut_int)); // send the future to the new thread
    chrono::seconds span(5);
    this_thread::sleep_for(span); // sleep for the current thread

    int convert_int = 10; 
    tool::Logging(class_name_.c_str(), "Set the share state.\n"); 
    prom_obj.set_value(convert_int); // fulfill the promise
    th_1.join();

    return ;
}

/**
 * @brief test with the future
 * 
 */
void MultiThreadLearning::FutureTest() {
    using namespace std;

    int input_prime = 700020007;
    // call function asynchronously
    future<bool> fut_bool = async(&MultiThreadLearning::IsPrime, this, input_prime);
    chrono::microseconds dur_1(1000);
    this_thread::sleep_for(dur_1);

    // do something while waiting for function to set future
    tool::Logging(class_name_.c_str(), "Checking, please wait\n");
    chrono::microseconds span(2000);
    while (fut_bool.wait_for(span) == future_status::timeout) {
        cerr << '.';
    }

    bool x = fut_bool.get();

    std::cout << "\n" << input_prime << " " << (x?"is":"is not") << " prime.\n";
    return ;
}