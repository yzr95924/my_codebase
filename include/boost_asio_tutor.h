/**
 * @file boost_asio_tutor.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief the boost asio lib tutorial
 * @version 0.1
 * @date 2021-10-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef BOOST_ASIO_TUTOR_H
#define BOOST_ASIO_TUTOR_H

#include "define.h"
#include <boost/asio.hpp> // the header for the asio classes
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class BoostAsioTutor {
    private:
        string class_name_ = "BoostAsioTutor";

        /**
         * @brief print current class name
         * 
         */
        void PrintCurrentClassName(const boost::system::error_code& /*e*/) {
            tool::Logging(class_name_.c_str(), "Test.\n");
            return ;
        }

    public:
        /**
         * @brief Construct a new Boost Asio Tutor object
         * 
         */
        BoostAsioTutor();

        /**
         * @brief Destroy the Boost Asio Tutor object
         * 
         */
        ~BoostAsioTutor();

        /**
         * @brief the sync timer
         * 
         */
        void SyncTimer();

        /**
         * @brief the async timer
         * 
         */
        void AsyncTime();
};

#endif