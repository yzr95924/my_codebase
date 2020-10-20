/**
 * @file compressTest.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief test Lz4 
 * @version 0.1
 * @date 2020-10-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/compress.h"

void Usage() {
    fprintf(stderr, "compressTest [-i inputFile] [-o outputFile] [-t com/decom].\n");
}

int main(int argc, char* argv[]) {
    const char* optstring = "i:t:";
    string inputPath;
    int type;
    int option;
    
    cout << "Size: optstring: " << sizeof(optstring) << endl;
    if (argc != 5) {
        fprintf(stderr, "Error: wrong argc number.\n");
        Usage();
        exit(EXIT_FAILURE);
    }

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'i':
                inputPath.assign(optarg);
                break;
            case 't':
                type = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "Error optopt: %c\n", optopt);
                fprintf(stderr, "Error opterr: %d\n", opterr);
                Usage();
                exit(EXIT_FAILURE);
        }
    }
    Compresser* test = new Compresser(inputPath);
    if (type == 1) {
        test->Compression();
    } else {
        test->Decompression();
    }

    delete test;
}