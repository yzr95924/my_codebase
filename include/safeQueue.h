/**
 * @file safeQueue.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief thread safe implementation of a queue using a std::queue
 * @version 0.1
 * @date 2021-10-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <queue>
#include <mutex>

using namespace std;

template <typename T>
class SafeQueue {
    private:
        // the queue 
        queue<T> queue_obj_; 

        // the lock of the queue
        mutex mutex_obj_;

    public:
        /**
         * @brief Construct a new Safe Queue object
         * 
         */
        SafeQueue() {

        }

        /**
         * @brief Destroy the Safe Queue object
         * 
         */
        ~SafeQueue() {

        }

        /**
         * @brief check whether the queue is empty
         * 
         * @return true yes
         * @return false no
         */
        bool CheckEmpty();

        /**
         * @brief Get the Queue Size object
         * 
         * @return int 
         */
        size_t GetQueueSize() {
            unique_lock<mutex> lock(mutex_obj_);
            return queue_obj_.size();
        }

        /**
         * @brief add a task to the queue
         * 
         * @param task the ref to the task
         */
        void Enqueue(T& task) {
            // add lock
            unique_lock<mutex> lock(mutex_obj_); 
            queue_obj_.push(task);
            return ;
        }

        /**
         * @brief pop a task from the queue
         * 
         * @param task the ref to the task
         * @return true success
         * @return false fails
         */
        bool Dequeue(T& task) {
            // add lock
            unique_lock<mutex> lock(mutex_obj_);

            if (queue_obj_.empty()) {
                return false;
            }

            task = move(queue_obj_.front());
            queue_obj_.pop();
            return true;
        }
        
    
};

#endif