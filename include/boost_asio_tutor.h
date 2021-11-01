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

        /**
         * @brief 
         * 
         * @param t pointer to the timer
         * @param count current counter stop the program when the timer fires for the sixth time
         */
        void Print(const boost::system::error_code& , 
            boost:: asio::deadline_timer* t, int* count) {
            if (*count < 5) {
                tool::Logging(class_name_.c_str(), "%d\n", *count);
                (*count)++;

                t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
                t->async_wait(boost::bind(&BoostAsioTutor::Print, this, 
                    boost::asio::placeholders::error, t, count));
            }

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

        /**
         * @brief the async time with more parameters
         * 
         */
        void AsyncTimeWithMoreParameters();
};

#endif