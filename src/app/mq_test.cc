/**
 * @file mq_test.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief test the MQ
 * @version 0.1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/define.h"
#include "../../include/constVar.h"
#include "../../include/message_queue/blocked_mq.h"
#include "../../include/message_queue/lck_free_mq.h"

#include <boost/thread/thread.hpp>

using namespace std;

const string my_name = "MQTest";

typedef struct {
    int x;
    int y;
} MQ_t;

uint32_t queue_size = 100;
BlockedMQ<MQ_t>* input_mq;

void RunTh1(BlockedMQ<MQ_t>* input_mq) {
    string my_name = "thread-1";
    tool::Logging(my_name.c_str(), "running.\n");
    for (int i = 0; i < queue_size * 2; i++) {
        MQ_t input_msg{i, i};
        tool::Logging(my_name.c_str(), "insert {%d, %d}\n", i, i);
        if (!input_mq->Push(input_msg)) {
            tool::Logging(my_name.c_str(), "insert chunk to output MQ error.\n");
            exit(EXIT_FAILURE);
        }
    }
    input_mq->_done = true;
    return ;
}

void RunTh2(BlockedMQ<MQ_t>* output_mq) {
    string my_name = "thread-2";
    tool::Logging(my_name.c_str(), "running.\n");
    bool job_done_flag = false;
    MQ_t tmp_msg;
    while (true) {
        // the main loop
        if (output_mq->_done && output_mq->IsEmpty()) {
            tool::Logging(my_name.c_str(), "no chunk in the MQ, all jobs are done.\n");
            job_done_flag = true;
        }

        if (output_mq->Pop(tmp_msg)) {
            tool::Logging(my_name.c_str(), "recv {%d, %d}\n", tmp_msg.x, tmp_msg.y);
        }

        if (job_done_flag) {
            break;
        }
    }

    tool::Logging(my_name.c_str(), "done.\n");
    return ;
}

int main(int argc, char* argv[]) {
    srand(tool::GetStrongSeed());
    input_mq = new BlockedMQ<MQ_t>(queue_size);

    boost::thread* tmp_th;
    vector<boost::thread*> th_list;

    boost::thread::attributes attrs;
    attrs.set_stack_size(THREAD_STACK_SIZE);

    tmp_th = new boost::thread(attrs, boost::bind(&RunTh1, input_mq));
    th_list.push_back(tmp_th);
    tmp_th = new boost::thread(attrs, boost::bind(&RunTh2, input_mq));
    th_list.push_back(tmp_th);

    for (auto it : th_list) {
        it->join();
    }

    for(auto it : th_list) {
        delete it;
    }

    delete input_mq;
    return 0;
}
