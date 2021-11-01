/**
 * @file boost_asio_tutor.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief boost asio tutorial
 * @version 0.1
 * @date 2021-10-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "../../include/boost_asio_tutor.h"
using namespace std;
/**
 * @brief Construct a new Boost Asio Tutor object
 * 
 */
BoostAsioTutor::BoostAsioTutor() {
    cout << "Init the boost asio tutorial." << endl;
}

/**
 * @brief Destroy the Boost Asio Tutor object
 * 
 */
BoostAsioTutor::~BoostAsioTutor() {
    cout << "Destory the boost asio tutorial." << endl;
}

/**
 * @brief the main process
 * 
 */
void BoostAsioTutor::SyncTimer() {
    // need to have at least one io_service obj, access to I/O functionality
    boost::asio::io_service io_ctx; 
    boost::asio::deadline_timer t(io_ctx, boost::posix_time::seconds(5));

    tool::Logging("BoostAsioTutor", "Wait start\n");
    // will not return until the timer has expired, 5 seconds after it was created 
    t.wait(); // not from when the wait starts
    tool::Logging("BoostAsioTutor", "Wait return\n");
    return ;
}

/**
 * @brief the async timer
 * 
 */
void BoostAsioTutor::AsyncTime() {
    boost::asio::io_service io_ctx;
    boost::asio::deadline_timer deadline(io_ctx, boost::posix_time::seconds(5));

    tool::Logging(class_name_.c_str(), "Wait start\n");
    // the callback handler
    deadline.async_wait(boost::bind(&BoostAsioTutor::PrintCurrentClassName, this, boost::asio::placeholders::error));
    tool::Logging(class_name_.c_str(), "Wait continue\n");
    io_ctx.run(); 
    return ;
}

/**
 * @brief the async time with more parameters
 * 
 */
void BoostAsioTutor::AsyncTimeWithMoreParameters() {
    boost::asio::io_service io_ctx;
    int count = 0;
    boost::asio::deadline_timer t(io_ctx, boost::posix_time::seconds(1));
    tool::Logging(class_name_.c_str(), "Wait start\n");
    t.async_wait(boost::bind(&BoostAsioTutor::Print, this, 
        boost::asio::placeholders::error, &t, &count));
    tool::Logging(class_name_.c_str(), "Wait continue\n");
    sleep(5);
    
    io_ctx.run();

    tool::Logging(this->class_name_.c_str(), "Final count is %d\n", count);

    return ;
}