/**
 * @file rabin_test.cc
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief benchmark of the rabin test
 * @version 0.1
 * @date 2022-06-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../../include/dedup/rabin_poly.h"
#include "../../include/dedup/new_rabin_poly.h"
#include "../../include/configure.h"

string my_name = "RabinTest";

void Usage() {
    fprintf(stderr, "%s -i [input file path].\n", my_name.c_str());
    return ;
}

Configure config("config.json");

int main(int argc, char* argv[]) {
    /* code */
    const char opt_string[] = "i:";
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

    uint64_t read_size = config.GetReadSize() * (1 << 20);
    uint8_t* read_buf = (uint8_t*) malloc(read_size * sizeof(uint8_t));
    struct timeval stime;
    struct timeval etime;
    
    NewRabin_t new_rabin_ctx;
    RabinCtx_t rabin_ctx;

    RabinFPUtil* rabin_util = new RabinFPUtil(config.GetChunkerSlidingWinSize());
    NewRabinUtil* new_rabin_util = new NewRabinUtil(config.GetChunkerSlidingWinSize());

    rabin_util->NewCtx(rabin_ctx);
    new_rabin_util->NewCtx(new_rabin_ctx);

    double rabin_time = 0;
    double new_rabin_time = 0;
    bool is_end = false;
    uint64_t pending_size = 0;
    while (!is_end) {
        input_file_hdl.read((char*)read_buf, read_size);
        pending_size = input_file_hdl.gcount();
        if (pending_size == 0) {
            break;
        }

        gettimeofday(&stime, NULL);
        for (size_t i = 0; i < pending_size; i++) {
            rabin_util->SlideOneByte(rabin_ctx, read_buf[i]);
        }
        gettimeofday(&etime, NULL);
        rabin_time += tool::GetTimeDiff(stime, etime);

        gettimeofday(&stime, NULL);
        for (size_t i = 0; i < pending_size; i++) {
            new_rabin_util->SlideOneByte(new_rabin_ctx, read_buf[i]);
        }        
        gettimeofday(&etime, NULL);
        new_rabin_time += tool::GetTimeDiff(stime, etime);
        is_end = input_file_hdl.eof();
    }

    rabin_util->FreeCtx(rabin_ctx);
    new_rabin_util->FreeCtx(new_rabin_ctx);

    fprintf(stdout, "--------Test Result--------\n");
    fprintf(stdout, "total file size (B): %lu\n", pending_size);
    fprintf(stdout, "rabin speed (MiB/s): %lf\n", static_cast<double>(pending_size) / 
        1024.0 / 1024.0 / rabin_time);
    fprintf(stdout, "new rabin speed (MiB/s): %lf\n", static_cast<double>(pending_size) / 
        1024.0 / 1024.0 / new_rabin_time);
    input_file_hdl.close();
    free(read_buf);
    delete rabin_util;
    delete new_rabin_util;
    return 0;
}