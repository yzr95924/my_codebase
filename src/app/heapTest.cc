/**
 * @file heapTest.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief test the performance of minHeap 
 * @version 0.1
 * @date 2020-10-31
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/define.h"
#include "../../include/ds/priorityHeap.h"


using namespace std;

int main(int argc, char* argv[]) {
    PriorityHeap<std::string, int, std::less<int>> minHeap;
    minHeap.add("1st", 1);
    minHeap.add("2rd", 2);
    minHeap.add("3rd", 0);
    cout << "Key: " << minHeap.top_k() << endl;
    cout << "Freq: " << minHeap.top_p() << endl;

    // update
    minHeap.update("3rd", 4);
    cout << "Key: " << minHeap.top_k() << endl;
    cout << "Freq: " << minHeap.top_p() << endl;

    // pop the heap
    minHeap.pop();
    cout << "Key: " << minHeap.top_k() << endl;
    cout << "Freq: " << minHeap.top_p() << endl;

    return 0;
}