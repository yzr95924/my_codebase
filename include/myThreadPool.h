/**
 * @file myThreadPool.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief the thread pool
 * @version 0.1
 * @date 2021-10-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// reference: https://wangpengcheng.github.io/2019/05/17/cplusplus_theadpool/
/**
 * 线程池管理器：初始化和创建线程，启动和停止线程，调配任务；管理线程池
 * 工作线程：线程池中等待并执行分配的任务
 * 任务接口：添加任务的接口，以提供工作线程调度任务的执行。
 * 任务队列：用于存放没有处理的任务，提供一种缓冲机制，同时具有调度功能，高优先级的任务放在队列前面
 */

/**
 * Task queue
 * Thread pool
 * Completed Tasks
 */

#ifndef MY_THREAD_POOL_H
#define MY_THREAD_POOL_H

class MyThreadPool {
    
};

#endif
