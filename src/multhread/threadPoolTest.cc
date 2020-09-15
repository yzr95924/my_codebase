#include <iostream>
#include <vector>
#include <chrono>

#include "../../include/threadPool.h"

using namespace std;

void ShowNumber(int inputVal) {
    std::cout << "Input Value: " << inputVal << endl;
}


int main()
{
    
    ThreadPool* pool = new ThreadPool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 4; ++i) {
        // results.emplace_back(
        //     pool.enqueue([i] {
        //         std::cout << "hello " << i << std::endl;
        //         std::this_thread::sleep_for(std::chrono::seconds(1));
        //         std::cout << "world " << i << std::endl;
        //         return i*i;
        //     })
        // );
        pool->enqueue(&ShowNumber, i);
    }

    getchar();

    cout << "Check point: start" << endl;

    delete pool;

    cout << "Check point: end" << endl;
    // for(auto && result: results)
    //     std::cout << result.get() << ' ';
    // std::cout << std::endl;
    
    return 0;
}