/**
 * @file cryptoTest.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief a benchmark framework for crypto tool 
 * @version 0.1
 * @date 2020-09-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/cryptoTool.h"

void usage() {
    fprintf(stderr, "cryptoBench [-e encryption type] [-h hash type]\n");
}

int main(int argc, char* argv[]) {
    const char* optstring = "e:h:f:o:";  
    int option;
    int cryptoType;
    int hashType;
    string path;
    string output;
    ifstream inputFile;
    ofstream outputFile;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'e':
                cryptoType = atoi(optarg);
                break;
            case 'h':
                hashType = atoi(optarg);
                break;
            case 'f':
                path = string(optarg);
                break;
            case 'o':
                output = string(optarg);
                break;
            case '?':
                fprintf(stderr, "Error optopt: %c\n", optopt);
                fprintf(stderr, "Error opterr: %d\n", opterr);
                usage();
                exit(EXIT_FAILURE);
        }
    }

    unique_ptr<CryptoTool> cipher(new CryptoTool(cryptoType, hashType));
    inputFile.open(path, std::ios::in | std::ios::binary);
    outputFile.open(output, std::ios::trunc | std::ios::binary);
    if (!inputFile.is_open()) {
        fprintf(stderr, "Cannot open this input file.\n");
        exit(EXIT_FAILURE);
    }

    uint8_t* key = (uint8_t*) malloc(sizeof(uint8_t) * 32);
    memset(key, 0, sizeof(int8_t) * 32);
    uint8_t hash[32];
    uint8_t* readBuffer = (uint8_t*) malloc(128 * 1024 * 1024);
    uint8_t* outputBuffer = (uint8_t*) malloc(128 * 1024 * 1024);
    bool end = false;
    double totalTime = 0;
    uint64_t fileSize = 0;
    while (!end) {
        inputFile.read((char*)readBuffer, sizeof(uint8_t) * 128 * 1024 * 1024);
        int len = inputFile.gcount();
        end = inputFile.eof();
        cipher->DecryptWithKey(readBuffer, len, key, outputBuffer);
        outputFile.write((char*)outputBuffer, len);
        fileSize += len;
    }

    free(readBuffer);
    free(outputBuffer);
    free(key);
    inputFile.close();
    outputFile.close();
    return 0;
}


