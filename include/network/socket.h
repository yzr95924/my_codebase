/**
 * @file socket.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface of socket 
 * @version 0.1
 * @date 2020-07-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MYPROJECT_SOCKET_H
#define MYPROJECT_SOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <unistd.h> 


using namespace std;

#define SERVER_TCP 0
#define CLIENT_TCP 1
#define SERVER_UDP 2
#define CLIENT_UDP 3

class Socket {
    private:
        // socket descriptor 
        int fd_;
        sockaddr_in addr_;
    public:
    
        /**
         * @brief Construct a new Socket object
         * 
         * @param type type UDP or TCP, server-side or client-side
         * @param ip ip address
         * @param port port number
         */
        Socket(const int type, string ip, short port);

        /**
         * @brief Construct a new Socket object
         * 
         * @param fd 
         * @param addr 
         */
        Socket(int fd, sockaddr_in addr); 


        Socket(){};

        /**
         * @brief Destroy the Socket object
         * 
         */
        ~Socket(){};

        /**
         * @brief initialize the socket connection
         * 
         * @param type UDP or TCP, server-side or client-side
         * @param ip ip address
         * @param port port number
         */
        void Init(const int type, string ip, short port);


        void Finish();

        bool Send(uint8_t* buffer, size_t sendSize);


        bool Recv(uint8_t* buffer, size_t& recvSize);
        
        /**
         * @brief listen to a port
         * 
         * @return Socket the refereed socket connected to the client
         */
        Socket Listen();

};


#endif // 