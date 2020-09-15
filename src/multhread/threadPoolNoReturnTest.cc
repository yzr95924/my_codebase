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
// #include <functional>

using namespace std;
void ShowNumber(int inputVal) {
   
    sleep(2);
     std::cout << "Input Value: " << inputVal << std::endl;
}


int main() {
    struct timeval sTime;
    struct timeval eTime;
    gettimeofday(&sTime, NULL);
    ThreadPool pool(4); // Creates 10 threads.
    int JOB_COUNT = 4;

    for (int i = 0; i < JOB_COUNT; ++i) {
        std::function<void()> targetfunction = std::bind(&ShowNumber, i);
        pool.AddJob(targetfunction);
    }
    // getchar();
    pool.JoinAll();
    std::cout << "Expected runtime: 8 seconds." << std::endl;
    gettimeofday(&eTime, NULL);
    double realTime = 0;
    realTime = tool::GetTimeDiff(sTime, eTime);
    std::cout << "real time: " << realTime << std::endl;
}


// void f(int &a,int &b,int &c){
//     std::cout<<"in function a = "<<a<<"  b = "<<b<<"  c = "<< c<< std::endl;
//     a += 1;
//     b += 10;
//     c += 100;
// }
 
// int main(){
 
//     int n1 = 1 ,n2 = 10,n3 = 100;
 
//     function<void()> f1 = bind(f,n1,n2,ref(n3));
 
//     f1();
//     cout<<"out function a = "<<n1<<"  b = "<<n2<<"  c = "<<n3<<endl;
//     f1();
//     cout<<"out function a = "<<n1<<"  b = "<<n2<<"  c = "<<n3<<endl;
 
//     return 0;
// }