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
void BoostAsioTutor::Run() {
    // need to have at least one io_service obj, access to I/O functionality
    boost::asio::io_service io_ctx; 
    boost::asio::deadline_timer t(io_ctx, boost::posix_time::seconds(5));

    tool::Logging("BoostAsioTutor", "Wait start\n");
    // will not return until the timer has expired, 5 seconds after it was created 
    t.wait(); // not from when the wait starts
    tool::Logging("BoostAsioTutor", "Wait return\n");
    return ;
}