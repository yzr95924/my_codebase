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
#include "../../include/message_queue/mq_factory.h"

#include <boost/thread/thread.hpp>

using namespace std;

const string my_name = "MQTest";

void Usage() {
    fprintf(stderr, "%s -t [MQ type]\n"
        "t [MQ type]:\n"
        "\tBlocked MQ: 0\n"
        "\tLck-free MQ: 1\n", my_name.c_str());
    return ;
}

typedef struct {
    int x;
    int y;
} MQ_t;

uint32_t queue_size = 1000;
uint32_t factor = 4;
AbsMQ<MQ_t>* input_mq;
MQFactory<MQ_t> mq_factory;
uint64_t sleep_time_ms = 10;

void RunTh1(AbsMQ<MQ_t>* input_mq) {
    string my_name = "thread-1";
    tool::Logging(my_name.c_str(), "running.\n");
    for (int i = 0; i < queue_size * 4; i++) {
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

void RunTh2(AbsMQ<MQ_t>* output_mq) {
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
            tool::Logging(my_name.c_str(), "wait {%d, %d}\n", tmp_msg.x, tmp_msg.y);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
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
    const char opt_str[] = "t:";

    if (argc != sizeof(opt_str)) {
        tool::Logging(my_name.c_str(), "wrong argc number.\n");
        Usage();
        exit(EXIT_FAILURE);
    }
    
    int option;
    uint32_t method_type = 0;
    while ((option = getopt(argc, argv, opt_str)) != -1) {
        switch (option) {
            case 't': {
                switch (atoi(optarg)) {
                    case BLOCKED_MQ: {
                        method_type = BLOCKED_MQ;
                        break;
                    }
                    case LCK_FREE_MQ: {
                        method_type = LCK_FREE_MQ;
                        break;
                    }
                    default: {
                        tool::Logging(my_name.c_str(), "wrong MQ type.\n");
                        Usage();
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            }
            case '?': {
                tool::Logging(my_name.c_str(), "error optopt: %c\n", optopt);
                tool::Logging(my_name.c_str(), "error opterr: %d\n", opterr);
                Usage();
                exit(EXIT_FAILURE);
            }
        }
    }

    input_mq = mq_factory.CreateMQ(method_type, queue_size);
    
    boost::thread* tmp_th;
    vector<boost::thread*> th_list;

    boost::thread::attributes attrs;
    attrs.set_stack_size(THREAD_STACK_SIZE);

    tmp_th = new boost::thread(attrs, boost::bind(&RunTh1, input_mq));
    th_list.push_back(tmp_th);
    tmp_th = new boost::thread(attrs, boost::bind(&RunTh2, input_mq));
    th_list.push_back(tmp_th);

    struct timeval stime;
    struct timeval etime;
    double total_time = 0;
    gettimeofday(&stime, NULL);
    for (auto it : th_list) {
        it->join();
    }
    gettimeofday(&etime, NULL);
    total_time += tool::GetTimeDiff(stime, etime);
    total_time -= (sleep_time_ms * queue_size * factor) / 1000.0;
    tool::Logging(my_name.c_str(), "total time: %lf\n", total_time);

    for(auto it : th_list) {
        delete it;
    }

    delete input_mq;
    return 0;
}
