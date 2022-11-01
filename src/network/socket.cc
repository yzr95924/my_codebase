/**
 * @file socket.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement the interface defined in socket.h
 * @version 0.1
 * @date 2020-07-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/network/socket.h"


Socket::Socket(const int type, string ip, short port) {
    this->Init(type, ip, port);
}

Socket::Socket(int fd, sockaddr_in addr) : fd_(fd), addr_(addr) {
   
}


void Socket::Init(const int type, string ip, short port) {
    memset(&this->addr_, 0, sizeof(this->addr_));
    this->addr_.sin_family = AF_INET;

    // convert to network byte order 
    this->addr_.sin_port = htons(port);

    switch(type) {
        case SERVER_TCP: {
            this->fd_ = socket(AF_INET, SOCK_STREAM, 0);
            // accept any connections
            this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);
            if (bind(this->fd_, (const struct sockaddr*)&this->addr_, sizeof(this->addr_)) != 0) {
                fprintf(stderr, "Socket: bind error\n");
                exit(EXIT_FAILURE);
            }
            // listen to the port
            if (listen(this->fd_, SOMAXCONN) != 0) {
                fprintf(stderr, "Socket: listen error\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case CLIENT_TCP: {
            this->fd_ = socket(AF_INET, SOCK_STREAM, 0);
            // convert ip string to network byte order
            if (inet_pton(AF_INET, ip.c_str(), &this->addr_.sin_addr) != 1) {
                fprintf(stderr, "Socket: ip convert error\n");
                exit(EXIT_FAILURE);
            }
            // connect to the server 
            if (connect(this->fd_, (const struct sockaddr*)&this->addr_, sizeof(this->addr_)) != 0) {
                fprintf(stderr, "Socket: connect error\n");
                exit(EXIT_FAILURE);                
            }
            break;
        }

        case SERVER_UDP: {
            this->fd_ = socket(AF_INET, SOCK_DGRAM, 0);
            if (inet_pton(AF_INET, ip.c_str(), &this->addr_.sin_addr) != 1) {
                fprintf(stderr, "Socket: ip convert error\n");
                exit(EXIT_FAILURE);
            }

            if (bind(this->fd_, (const struct sockaddr*)&this->addr_, sizeof(this->addr_)) != 0) {
                fprintf(stderr, "Socket: bind error\n");
                fprintf(stderr, "May cause by shutdown server before client\n");
                fprintf(stderr, "Wait for 30 sec and try again\n");
                exit(EXIT_FAILURE);
            }
            break;
        }

        case CLIENT_UDP: {
            this->fd_ = socket(AF_INET, SOCK_DGRAM, 0);
            inet_pton(AF_INET, ip.c_str(), &this->addr_.sin_addr);
            if (connect(this->fd_, (const struct sockaddr*)&this->addr_, sizeof(this->addr_)) != 0) {
                fprintf(stderr, "Socket: connect error\n");
                exit(EXIT_FAILURE);
            }
            break;
        } 

        default: {
            fprintf(stderr, "Socket: Error type for socket\n");
            exit(EXIT_FAILURE);
        }
    }

    return ;
}

void Socket::Finish() {
    close(this->fd_);
}

Socket Socket::Listen() {
    int clientFd;
    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));

    uint32_t addrSize = sizeof(clientAddr);
    clientFd = accept(this->fd_, (struct sockaddr*)&clientAddr, (socklen_t*)&addrSize);
    if (clientFd > 0) {
        fprintf(stderr, "Socket: Client connect fd: %d\n", clientFd);
        return Socket(clientFd, clientAddr);
    } else {
        fprintf(stderr, "Socket: accept error\n");
        exit(EXIT_FAILURE);
    }
}


bool Socket::Send(uint8_t* buf, size_t send_size) {
    size_t finish_size = 0;
    int len;
    int s = write(this->fd_, (const uint8_t*)&send_size, sizeof(send_size));
    if (s == -1) {
        fprintf(stderr, "Socket: send error\n");
        this->Finish();
        return false;
    }

    while (finish_size < send_size) {
        len = write(this->fd_, buf + finish_size, send_size - finish_size);
        if (len == -1) {
            fprintf(stderr, "Socket: send error %u\n", errno);
            this->Finish();
            return false;
        }
        finish_size += len;
    }
    if (finish_size != send_size) {
        fprintf(stderr, "Socket: finish sent size error\n");
        this->Finish();
        return false;
    }
    return true;
}


bool Socket::Recv(uint8_t* buf, size_t& exp_recv_size) {
    size_t recv_size = 0;
    int read_byte_num = 0;
    read_byte_num = read(this->fd_, (uint8_t*)&exp_recv_size,
        sizeof(exp_recv_size));
    if (read_byte_num == 0) {
        fprintf(stderr, "Socket: size = 0\n");
        this->Finish();
        return false;
    }

    if(exp_recv_size == 0) {
        fprintf(stderr, "Socket: recvSize = 0\n");
        this->Finish();
        return false;
    }

    while (recv_size < exp_recv_size) {
        read_byte_num = read(this->fd_, buf + recv_size, exp_recv_size - recv_size);
        // should check errno here
        if (read_byte_num <= 0) {
            fprintf(stderr, "Socket: recv errno: %u\n", errno);
            this->Finish();
            return false;
        }
        recv_size += read_byte_num;
    }

    if (recv_size != exp_recv_size) {
        fprintf(stderr, "Socket: finish received size error\n");
        this->Finish();
        return false;
    }
    return true;
}