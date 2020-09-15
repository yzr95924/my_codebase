/**
 * @file threadPoolNoReturnTest.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief test for the thread pool without returning value
 * @version 0.1
 * @date 2020-08-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/threadPoolNoReturn.h"

#include <iostream>
#include <chrono>
// #include <functional>

using namespace std;
// void ShowNumber(int inputVal) {
//     std::cout << "Input Value: " << inputVal << std::endl;
// }


// int main() {
//     ThreadPool pool(4); // Creates 10 threads.
//     int JOB_COUNT = 4;

//     for (int i = 0; i < JOB_COUNT; ++i) {
//         std::function<void()> targetfunction = std::bind(&ShowNumber, i);
//         pool.AddJob(targetfunction);
//     }
//     pool.JoinAll();
//     getchar();
//     std::cout << "Expected runtime: 10 seconds." << std::endl;
// }


void f(int &a,int &b,int &c){
    std::cout<<"in function a = "<<a<<"  b = "<<b<<"  c = "<< c<< std::endl;
    a += 1;
    b += 10;
    c += 100;
}
 
int main(){
 
    int n1 = 1 ,n2 = 10,n3 = 100;
 
    function<void()> f1 = bind(f,n1,n2,ref(n3));
 
    f1();
    cout<<"out function a = "<<n1<<"  b = "<<n2<<"  c = "<<n3<<endl;
    f1();
    cout<<"out function a = "<<n1<<"  b = "<<n2<<"  c = "<<n3<<endl;
 
    return 0;
}