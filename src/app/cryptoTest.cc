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
struct timeval start_time;
struct timeval end_time;

static const uint64_t GB = (1 << 30);

void reverseBytes(uint8_t* start, int size) {
    unsigned char *lo = start;
    unsigned char *hi = start + size - 1;
    unsigned char swap;
    while (lo < hi) {
        swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }
}

void usage() {
    fprintf(stderr, "cryptoBench [-s message size (B)]\n");
    return ;
}

int main(int argc, char* argv[]) {
    uint64_t TEST_DATA_SIZE = GB * 4;
    srand(time(NULL));
    const char optstring[] = "s:";  
    int option;
    int64_t read_buffer_size;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 's':
                read_buffer_size = atoll(optarg);
                break;
            case '?':
                fprintf(stderr, "Error optopt: %c\n", optopt);
                fprintf(stderr, "Error opterr: %d\n", opterr);
                usage();
                exit(EXIT_FAILURE);
        }
    }
    
    if (argc != sizeof(optstring)) {
        fprintf(stderr, "operator number wrong: %d != 6\n", argc);
        exit(EXIT_FAILURE);
    }


    uint64_t TEST_TIME = TEST_DATA_SIZE / read_buffer_size;
    cout << "TEST_TIME: " << TEST_DATA_SIZE << endl; 
    CryptoTool* cipher = new CryptoTool(AES_256_GCM, SHA_256);

    uint8_t* key = (uint8_t*) malloc(sizeof(uint8_t) * 32);
    memset(key, 1, sizeof(int8_t) * 32);
    uint8_t hash[32];
    uint8_t* output_buffer = (uint8_t*) malloc(read_buffer_size);
    uint8_t* input_buffer = (uint8_t*) malloc(read_buffer_size);
    uint8_t* tmp_buffer = (uint8_t*) malloc(read_buffer_size);
    int* tmp_ptr = (int*)input_buffer;
    for (int i = 0; i < (read_buffer_size / sizeof(int)); i++) {
        *tmp_ptr = rand();
        tmp_ptr++;
    }

    tool::Logging(__FILE__, "Check encryption/decryption correctness.\n");
    cipher->EncryptWithKey(input_buffer, read_buffer_size, key, output_buffer);
    cipher->DecryptWithKey(output_buffer, read_buffer_size, key, tmp_buffer);
    if (0 != memcmp(input_buffer, tmp_buffer, read_buffer_size)) {
        tool::Logging(__FILE__, "Encryption/decryption not match.\n");
        exit(EXIT_FAILURE);
    } else {
        tool::Logging(__FILE__, "Encryption/decryption is correct.\n");
    }

    double enc_time = 0;
    double dec_time = 0;
    double hash_time = 0;
    double hmac_time = 0;
    double aes_cmc_time = 0;
    tool::Logging(__FILE__, "Start test.\n");
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < TEST_TIME; i++) {
        cipher->EncryptWithKey(input_buffer, read_buffer_size, key, output_buffer);
    }
    gettimeofday(&end_time, NULL);
    enc_time += tool::GetTimeDiff(start_time, end_time);
    tool::Logging(__FILE__, "Finish encryption test.\n");

    gettimeofday(&start_time, NULL);
    for (int i = 0; i < TEST_TIME; i++) {
        cipher->GenerateHash(input_buffer, read_buffer_size, hash);
    }
    gettimeofday(&end_time, NULL);
    hash_time += tool::GetTimeDiff(start_time, end_time);
    tool::Logging(__FILE__, "Finish hashing test.\n");

    gettimeofday(&start_time, NULL);
    for (int i = 0; i < TEST_TIME; i++) {
        cipher->DecryptWithKey(output_buffer, read_buffer_size, key, input_buffer); 
    }
    gettimeofday(&end_time, NULL);
    dec_time += tool::GetTimeDiff(start_time, end_time);
    tool::Logging(__FILE__, "Finish decryption test.\n");

    // gettimeofday(&start_time, NULL);
    // for (int i = 0; i < TEST_TIME; i++) {
    //     cipher->GenerateHMAC(input_buffer, read_buffer_size, hash); 
    // }
    // gettimeofday(&end_time, NULL);
    // hmac_time += tool::GetTimeDiff(start_time, end_time);
    // tool::Logging(__FILE__, "Finish hmac test.\n");

    // gettimeofday(&start_time, NULL); 
    // for (int i = 0; i < TEST_TIME; i++) {
    //     cipher->EncryptWithKey(input_buffer, read_buffer_size, key, tmp_buffer);
    //     // reverse(tmp_buffer, tmp_buffer + read_buffer_size);
    //     // reverse(tmp_buffer, tmp_buffer + read_buffer_size);
    //     reverseBytes(tmp_buffer, read_buffer_size);
    //     cipher->EncryptWithKey(tmp_buffer, read_buffer_size, key, output_buffer);
    // }
    // gettimeofday(&end_time, NULL);
    // aes_cmc_time += tool::GetTimeDiff(start_time, end_time);
    // tool::Logging(__FILE__, "Finish AES-CMC test.\n");

    tool::Logging(__FILE__, "Test done.\n");
    tool::Logging(__FILE__, "Encryption (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / enc_time);
    tool::Logging(__FILE__, "Hashing (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / hash_time);
    tool::Logging(__FILE__, "Decryption (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / dec_time);
    // tool::Logging(__FILE__, "HMAC (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / hmac_time);
    // tool::Logging(__FILE__, "AES-CMC (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / aes_cmc_time);
    
    free(input_buffer);
    free(output_buffer);
    free(tmp_buffer);
    free(key);
    delete cipher;
    return 0;
}