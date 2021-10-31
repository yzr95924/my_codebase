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
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class BoostAsioTutor {
    private:

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
         * @brief the main process
         * 
         */
        void Run();
};

#endif