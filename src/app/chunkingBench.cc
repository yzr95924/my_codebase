/**
 * @file test.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief a test app for different chunking algorithm 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#include "../../include/rfChunking.h"
#include "../../include/fixChunking.h"
#include "../../include/fastChunkingTest.h"

void usage() {
    fprintf(stderr, "test [-t chunking type] [-f fileName] [-l minChunkSize] [-h maxChunkSize] [-a avgChunkSize].\n");
}


int main(int argc, char* argv[]) {

    const char* optstring = "f:l:h:a:t:";
    int option;

    string filePath;
    int minChunkSize;
    int maxChunkSize;
    int avgChunkSize;
    int type;

    if (argc != 11) {
        fprintf(stderr, "Error: wrong argc number.\n");
        usage();
        exit(EXIT_FAILURE);
    }


    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'f':
                filePath.assign(optarg);
                break;
            case 'l':
                minChunkSize = atoi(optarg) * 1024;
                break;
            case 'h':
                maxChunkSize = atoi(optarg) * 1024;
                break;
            case 'a':
                avgChunkSize = atoi(optarg) * 1024;
                break;
            case 't':
                type = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "Error optopt: %c\n", optopt);
                fprintf(stderr, "Error opterr: %d\n", opterr);
                usage();
                exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "------configuration------\n");
    fprintf(stderr, "file path: %s\n", filePath.c_str());
    fprintf(stderr, "min chunk size: %d\n", minChunkSize);
    fprintf(stderr, "max chunk size: %d\n", maxChunkSize);
    fprintf(stderr, "avg chunk size: %d\n", avgChunkSize);
    fprintf(stderr, "--------------------------\n");

    switch (type) {
        case CHUNKER_VAR_SIZE_TYPE: {
            RFChunking* rfChunker = new RFChunking(filePath, minChunkSize,
                maxChunkSize, avgChunkSize);
            rfChunker->Chunking();
            delete rfChunker;
            break;
        }
        case CHUNKER_FIX_SIZE_TYPE: {
            FixChunking* fixChunker = new FixChunking(filePath, minChunkSize,
                maxChunkSize, avgChunkSize);
            fixChunker->Chunking();
            delete fixChunker;
            break;
        }
        case FAST_CDC: {
            FastCDC* fastCDCChunker = new FastCDC(filePath, minChunkSize, 
                maxChunkSize, avgChunkSize);
            fastCDCChunker->Chunking();
            delete fastCDCChunker;
            break;
        }
        default: {
            fprintf(stderr, "chunkerBench: wrong chunker type: %d\n", type);
            break;
        }
    }

    return 0;
}