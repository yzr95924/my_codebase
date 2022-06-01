/**
 * @file client.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief implement a simple client program
 * @version 0.1
 * @date 2021-01-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../../include/network/ssl_conn.h"
#include "../../include/configure.h"


int main(int argc, char* argv[])
{
    pair<int, SSL*> newSSLConnectionRecord;
    // config the parameter
    int port = 16666;
    string serverIp = "127.0.0.1";
    SSLConnection* sslConnectionHandler = new SSLConnection(serverIp, port, IN_CLIENT_SIDE);
    newSSLConnectionRecord = sslConnectionHandler->ConnectSSL();

    int sendData = 95924;
    fprintf(stderr, "Send data is %d\n", sendData);
    if (!sslConnectionHandler->SendData(newSSLConnectionRecord.second, (uint8_t*)&sendData, sizeof(int))) {
        fprintf(stderr, "Send data error.\n");
        exit(EXIT_FAILURE);
    }

    sendData = 10000;    
    fprintf(stderr, "Send data is %d\n", sendData);
    if (!sslConnectionHandler->SendData(newSSLConnectionRecord.second, (uint8_t*)&sendData, sizeof(int))) {
        fprintf(stderr, "Send data error.\n");
        exit(EXIT_FAILURE);
    }

    // if (!sslConnectionHandler->ReceiveData(newSSLConnectionRecord.second, (uint8_t*)&recvData, recvDataSize)) {
    //     fprintf(stderr, "The connection is closed.\n");
    // }

    // fprintf(stderr, "Receive data is %d\n", recvData);
    
    sslConnectionHandler->Finish(newSSLConnectionRecord);
    delete sslConnectionHandler;
    fprintf(stderr, "Finish the test.\n");
    return 0;
}