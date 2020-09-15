
#include "../../include/threadPool.h"

using namespace std;

void ShowNumber(int inputVal) {
    sleep(2);
    std::cout << "Input Value: " << inputVal << endl;
}


int main()
{
    
    ThreadPool* pool = new ThreadPool(4);
    std::vector< std::future<int> > results;

    struct timeval sTime;
    struct timeval eTime;

    gettimeofday(&sTime, NULL);

    for(int i = 0; i < 4; ++i) {
        pool->enqueue(&ShowNumber, i);
    }

    pool->wait_until_nothing_in_flight();

    gettimeofday(&eTime, NULL);

    double totalTime;
    totalTime = tool::GetTimeDiff(sTime, eTime);

    cout << "Expected time: 8s" << endl;
    cout << "Real total time: " << totalTime << endl;

    delete pool;

    return 0;
}