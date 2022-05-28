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

#include "../../include/compression/compress_util.h"

string my_name = "CompressTest";

void Usage() {
    fprintf(stderr, "CompressTest -i [inputFile path] -t [compression algo type] -s [chunk size (KiB)] -l [compression level].\n"
        "\t-t [compression algo type]:\n"
        "\t0: LZ4\n"
        "\t1: ZSTD\n"
        "\t2: ZLIB\n");
    return ;
}

int main(int argc, char* argv[]) {
    const char opt_string[] = "i:t:s:l:";
    string input_path;
    uint32_t chunk_size = 0;
    uint32_t level = 0;
    int type;
    int option;

    if (argc != sizeof(opt_string)) {
        tool::Logging(my_name.c_str(), "wrong argc number.\n");
        Usage();
        exit(EXIT_FAILURE);
    }

    while ((option = getopt(argc, argv, opt_string)) != -1) {
        switch (option) {
            case 'i': {
                input_path.assign(optarg);
                break;
            }
            case 't': {
                switch(atoi(optarg)) {
                    case LZ4_COMPRESS_TYPE: {
                        tool::Logging(my_name.c_str(), "using LZ4.\n");
                        type = LZ4_COMPRESS_TYPE;
                        break;
                    }
                    case ZSTD_COMPRESS_TYPE: {
                        tool::Logging(my_name.c_str(), "using ZSTD.\n");
                        type = ZSTD_COMPRESS_TYPE;
                        break;
                    }
                    case ZLIB_COMPRESS_TYPE: {
                        tool::Logging(my_name.c_str(), "using ZLIB.\n");
                        type = ZLIB_COMPRESS_TYPE;
                        break;
                    }
                    default: {
                        tool::Logging(my_name.c_str(), "wrong compression algo type");
                        Usage();
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            }
            case 'l': {
                level = atoi(optarg);
                break;
            }
            case 's': {
                chunk_size = atoi(optarg) * 1024;
                if (chunk_size > MAX_CHUNK_SIZE) {
                    tool::Logging(my_name.c_str(), "the input chunk size (%u) is higher than %u\n",
                        chunk_size, MAX_CHUNK_SIZE);
                    exit(EXIT_FAILURE);
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

    CompressUtil* compress_util = new CompressUtil(type, level);

    uint8_t output_chunk[MAX_CHUNK_SIZE] = {0};
    uint8_t input_chunk[MAX_CHUNK_SIZE] = {0};
    uint8_t uncompressed_chunk[MAX_CHUNK_SIZE] = {0};

    ifstream input_file_hdl;
    input_file_hdl.open(input_path, ios_base::in | ios_base::binary);
    if (!input_file_hdl.is_open()) {
        tool::Logging(my_name.c_str(), "cannot open %s.\n", input_path.c_str());
        exit(EXIT_FAILURE);
    }

    bool is_end = false;
    bool correct_test = true;
    int read_byte_num = 0;
    struct timeval stime;
    struct timeval etime;
    double compressed_time = 0;
    double decompressed_time = 0;
    uint64_t total_uncompressed_size = 0;
    uint64_t total_compressed_size = 0;
    while (!is_end) {
        input_file_hdl.read((char*)input_chunk, chunk_size);
        read_byte_num = input_file_hdl.gcount();
        is_end = input_file_hdl.eof();
        if (read_byte_num == 0) {
            break;
        }

        total_uncompressed_size += read_byte_num;

        uint32_t compressed_chunk_size = 0;
        gettimeofday(&stime, NULL);
        compress_util->CompressOneChunk(input_chunk, read_byte_num, output_chunk,
            &compressed_chunk_size);
        gettimeofday(&etime, NULL);
        compressed_time += tool::GetTimeDiff(stime, etime);

        total_compressed_size += compressed_chunk_size;

        uint32_t uncompressed_size = 0; 
        gettimeofday(&stime,NULL);
        compress_util->DecompressOneChunk(output_chunk, compressed_chunk_size, 
            uncompressed_chunk, &uncompressed_size);
        gettimeofday(&etime, NULL);
        decompressed_time += tool::GetTimeDiff(stime, etime);

        if (correct_test) {
            // check the size first
            tool::Logging(my_name.c_str(), "read byte size: %lu\n", read_byte_num);
            if (uncompressed_size != read_byte_num) {
                tool::Logging(my_name.c_str(), "uncompressed length is not correct.\n");
                exit(EXIT_FAILURE);
            } else {
                if (memcmp(input_chunk, uncompressed_chunk, read_byte_num) != 0) {
                    tool::Logging(my_name.c_str(), "the uncompressed content is wrong.\n");
                    exit(EXIT_FAILURE);
                }
            }
            correct_test = false;
            tool::Logging(my_name.c_str(), "correctness test is passed.\n");
        }

        if (is_end) {
            break;
        }
    }

    fprintf(stdout, "--------Test Result--------\n");
    fprintf(stdout, "total uncompressed size (B): %lu\n", total_uncompressed_size);
    fprintf(stdout, "total compressed size (B): %lu\n", total_compressed_size);
    fprintf(stdout, "compression ratio: %lf\n", static_cast<double>(total_uncompressed_size) / 
        total_compressed_size);
    fprintf(stdout, "compression speed (MiB/s): %lf\n", static_cast<double>(total_uncompressed_size) / 
        1024.0 / 1024.0 / compressed_time);
    fprintf(stdout, "decompression speed (MiB/s): %lf\n", static_cast<double>(total_uncompressed_size) / 
        1024.0 / 1024.0 / decompressed_time);
    delete compress_util;
}