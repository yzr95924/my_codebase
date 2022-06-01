/**
 * @file server.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief a simple server program to test ssl connection
 * @version 0.1
 * @date 2021-01-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <signal.h> // to catch the signal
#include "../../include/network/ssl_conn.h"

SSLConnection* sslHandler;

void CTRLC(int s) {
    fprintf(stderr, "Terminate the server with keyboard interrupt, caught signal %d\n", s);
    delete sslHandler;
    fprintf(stderr, "Finish the test.\n");
    exit(EXIT_SUCCESS);
    return ;
}

int main(int argc, char* argv[]) {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sigIntHandler, 0);

    sigIntHandler.sa_handler = CTRLC;
    sigaction(SIGKILL, &sigIntHandler, 0);
    sigaction(SIGINT, &sigIntHandler, 0);
    
    string serverIp = "127.0.0.1";
    int port = 16666;
    sslHandler = new SSLConnection(serverIp, port, IN_SERVER_SIDE);
    pair<int, SSL*> connectionRecord;

    int recvData;
    uint32_t recvDataSize;

    while (true) {
        connectionRecord = sslHandler->ListenSSL();
        while (true) {
            if (!sslHandler->ReceiveData(connectionRecord.second, (uint8_t*)&recvData, recvDataSize)) {
                fprintf(stderr, "The connection is closed.\n");
                sslHandler->ClearAcceptedClientSd(connectionRecord.second);
                break;
            }
            fprintf(stderr, "Receive data is %d\n", recvData);
            // recvData += 100;
            // if (!sslHandler->SendData(connectionRecord.second, (uint8_t*)&recvData, sizeof(int))) {
            //     fprintf(stderr, "Send data error.\n");
            //     exit(EXIT_FAILURE);
            // }
        }
        fprintf(stderr, "Finish the current connection, wait for other connections.\n");
    }
    
    return 0;
}