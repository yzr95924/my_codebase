/**
 * @file chunking_test.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief a benchmark tool for chunking
 * @version 0.1
 * @date 2022-06-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/dedup/abs_chunker.h"
#include "../../include/dedup/fix_chunker.h"
#include "../../include/dedup/rabin_chunker.h"
#include "../../include/dedup/fastcdc_chunker.h"
#include "../../include/dedup/chunker_factory.h"
#include "../../include/dedup/finesse_util.h"
#include "../../include/constVar.h"
#include "../../include/crypto/crypto_util.h"

string my_name = "ChunkingTest";

Configure config("config.json");

void Usage() {
    fprintf(stderr, "%s -i [input file path] -t [chunking algo type].\n"
        "-t [chunking algo type]:\n"
        "\t0: Fixed-size\n"
        "\t1: Rabin-fingerprint\n"
        "\t2: FastCDC\n", my_name.c_str());
    return ;
}

int main(int argc, char* argv[]) {
    /* code */
    const char opt_string[] = "i:t:";
    string input_path;
    int type;

    if (argc != sizeof(opt_string)) {
        tool::Logging(my_name.c_str(), "wrong argc number.\n");
        Usage();
        exit(EXIT_FAILURE);
    }

    int option;
    while ((option = getopt(argc, argv, opt_string)) != -1) {
        switch (option) {
            case 'i': {
                input_path.assign(optarg);
                break;
            }
            case 't': {
                switch (atoi(optarg)) {
                    case FIXED_SIZE_CHUNKING: {
                        tool::Logging(my_name.c_str(), "using Fixed-size chunking.\n");
                        type = FIXED_SIZE_CHUNKING;
                        break;
                    }
                    case RABIN_FP_CHUNKING: {
                        tool::Logging(my_name.c_str(), "using Rabin-fingerprint chunking.\n");
                        type = RABIN_FP_CHUNKING;
                        break;
                    }
                    case FAST_CDC: {
                        tool::Logging(my_name.c_str(), "using FastCDC chunking.\n");
                        type = FAST_CDC;
                        break;
                    }
                    case FSL_TRACE: {
                        tool::Logging(my_name.c_str(), "using FSL-Trace chunking.\n");
                        type = FSL_TRACE;
                        break;
                    }
                    case MS_TRACE: {
                        tool::Logging(my_name.c_str(), "using MS-Trace chunking.\n");
                        type = MS_TRACE;
                        break;
                    }
                    default: {
                        tool::Logging(my_name.c_str(), "wrong chunking algo type.\n");
                        Usage();
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            }
            case '?': {
                tool::Logging(my_name.c_str(), "error optopt: %c\n", optopt);
                tool::Logging(my_name.c_str(), "error opterr: %d\n", opterr);
                Usage();
                exit(EXIT_FAILURE);
            }
        }
    }

    ifstream input_file_hdl;
    input_file_hdl.open(input_path, ios_base::in | ios_base::binary);
    if (!input_file_hdl.is_open()) {
        tool::Logging(my_name.c_str(), "cannot open %s.\n", input_path.c_str());
        exit(EXIT_FAILURE);
    }

    struct timeval stime;
    struct timeval etime;
    struct timeval s_finesse_time;
    struct timeval e_finesse_time;

    uint64_t super_feature_per_chunk = 3;
    uint64_t feature_per_chunk = 12;
    uint64_t feature_per_super_feature = 4;

    AbsChunker* test_chunker; 
    FinesseUtil* finesse_util;
    RabinCtx_t rabin_ctx;
    RabinFPUtil* rabin_util = new RabinFPUtil(config.GetSimilarSlidingWinSize());
    finesse_util = new FinesseUtil(super_feature_per_chunk, feature_per_chunk,
        feature_per_super_feature);
    rabin_util->NewCtx(rabin_ctx);

    switch (type) {
        case FIXED_SIZE_CHUNKING: {
            test_chunker = new FixChunker();
            break;
        }
        case RABIN_FP_CHUNKING: {
            test_chunker = new RabinChunker();
            break;
        }
        case FAST_CDC: {
            test_chunker = new FastCDC();
            break;
        }
    }

    double chunking_time = 0;
    double finesse_time = 0;
    bool is_end = false;
    while (!is_end) {
        uint64_t pending_size = 0;
        pending_size = test_chunker->LoadDataFromFile(input_file_hdl);
        if (pending_size == 0) {
            break;
        }

        uint8_t chunk_buf[MAX_CHUNK_SIZE] = {0};
        uint32_t chunk_size;
        uint64_t features[super_feature_per_chunk];
        while (true) {
            gettimeofday(&stime, NULL);
            chunk_size = test_chunker->GenerateOneChunk(chunk_buf);
            gettimeofday(&etime, NULL);
            chunking_time += tool::GetTimeDiff(stime, etime);

            if (!chunk_size) {
                break;
            }

            // TODO: process the chunk here
            // cout << chunk_size << endl;
            gettimeofday(&s_finesse_time, NULL);
            finesse_util->ExtractFeature(rabin_ctx, chunk_buf, chunk_size,
                features);
            for (uint32_t i = 0; i < super_feature_per_chunk; i++) {
                cout << features[i] << endl;
            }
            gettimeofday(&e_finesse_time, NULL);
            finesse_time += tool::GetTimeDiff(s_finesse_time, e_finesse_time);
        }
        is_end = input_file_hdl.eof();
    }

    fprintf(stdout, "--------Test Result--------\n");
    fprintf(stdout, "total file size (B): %lu\n", test_chunker->_total_file_size);
    fprintf(stdout, "total chunk num: %lu\n", test_chunker->_total_chunk_num);
    fprintf(stdout, "chunking speed (MiB/s): %lf\n", static_cast<double>(test_chunker->_total_file_size) / 
        1024.0 / 1024.0 / chunking_time);
    fprintf(stdout, "finesse speed (MiB/s): %lf\n", static_cast<double>(test_chunker->_total_file_size) / 
        1024.0 / 1024.0 / finesse_time);
    input_file_hdl.close();
    delete test_chunker;
    delete finesse_util;
    rabin_util->FreeCtx(rabin_ctx);
    delete rabin_util;
    return 0;
}