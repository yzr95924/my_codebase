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

#include "../../include/cryptoPrimitive.h"
struct timeval start_time;
struct timeval end_time;

const string myName = "CryptoBench";

static const uint64_t GB = (1 << 30);

void usage() {
    tool::Logging(myName.c_str(), "cryptoBench [-s message size (byte)]\n");
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
                tool::Logging(myName.c_str(), "error optopt: %c\n", optopt);
                tool::Logging(myName.c_str(), "error opterr: %d\n", opterr);
                usage();
                exit(EXIT_FAILURE);
        }
    }
    
    if (argc != sizeof(optstring)) {
        tool::Logging(myName.c_str(), "operator number wrong: %d != 6\n", argc);
        exit(EXIT_FAILURE);
    }

    // prepare
    uint64_t TEST_TIME = TEST_DATA_SIZE / read_buffer_size;
    CryptoPrimitive* cipher = new CryptoPrimitive(AES_256_CTR, SHA_256);
    EVP_CIPHER_CTX* cipherCtx = EVP_CIPHER_CTX_new();
    EVP_MD_CTX* mdCtx = EVP_MD_CTX_new();

    uint8_t* key = (uint8_t*) malloc(sizeof(uint8_t) * 32);
    memset(key, 1, sizeof(uint8_t) * 32);
    uint8_t hash[CHUNK_HASH_SIZE];
    uint8_t* output_buffer = (uint8_t*) malloc(read_buffer_size + CRYPTO_BLOCK_SIZE);
    uint8_t* input_buffer = (uint8_t*) malloc(read_buffer_size + CRYPTO_BLOCK_SIZE);
    uint8_t* tmp_buffer = (uint8_t*) malloc(read_buffer_size + CRYPTO_BLOCK_SIZE);
    int* tmp_ptr = (int*)input_buffer;
    for (int i = 0; i < (read_buffer_size / sizeof(int)); i++) {
        *tmp_ptr = rand();
        tmp_ptr++;
    }

    // preparation done
    // tool::Logging(myName.c_str(), "check encryption/decryption correctness.\n");
    // cipher->EncryptWithKey(cipherCtx, input_buffer, read_buffer_size, key, 
    //     output_buffer);
    // cipher->DecryptWithKey(cipherCtx, output_buffer, read_buffer_size, key, 
    //     tmp_buffer);
    // if (0 != memcmp(input_buffer, tmp_buffer, read_buffer_size)) {
    //     tool::Logging(myName.c_str(), "encryption/decryption not match.\n");
    //     exit(EXIT_FAILURE);
    // } else {
    //     tool::Logging(myName.c_str(), "encryption/decryption is correct.\n");
    // }

    double enc_time = 0;
    double dec_time = 0;
    double hash_time = 0;
    double hmac_time = 0;
    tool::Logging(myName.c_str(), "start test.\n");
    uint8_t tmpCipher[MAX_CHUNK_SIZE];
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < TEST_TIME; i++) {
        cipher->EncryptWithKey(cipherCtx, input_buffer, read_buffer_size, 
            key, tmpCipher);
        cipher->TestAESECBEnc(cipherCtx, tmpCipher, read_buffer_size,
            key, output_buffer);
    }
    gettimeofday(&end_time, NULL);
    enc_time += tool::GetTimeDiff(start_time, end_time);
    tool::Logging(myName.c_str(), "finish encryption test.\n");

    // gettimeofday(&start_time, NULL);
    // for (int i = 0; i < TEST_TIME; i++) {
    //     cipher->GenerateHash(mdCtx, input_buffer, read_buffer_size, 
    //         hash);
    // }
    // gettimeofday(&end_time, NULL);
    // hash_time += tool::GetTimeDiff(start_time, end_time);
    // tool::Logging(myName.c_str(), "finish hashing test.\n");

    // gettimeofday(&start_time, NULL);
    // for (int i = 0; i < TEST_TIME; i++) {
    //     cipher->GenerateHMAC(mdCtx, input_buffer, read_buffer_size,
    //         hash);
    // }
    // gettimeofday(&end_time, NULL);
    // hmac_time += tool::GetTimeDiff(start_time, end_time);
    // tool::Logging(myName.c_str(), "finish hmac test.\n");

    // gettimeofday(&start_time, NULL);
    // for (int i = 0; i < TEST_TIME; i++) {
    //     cipher->DecryptWithKey(cipherCtx, output_buffer, read_buffer_size, 
    //         key, input_buffer); 
    // }
    // gettimeofday(&end_time, NULL);
    // dec_time += tool::GetTimeDiff(start_time, end_time);
    // tool::Logging(myName.c_str(), "finish decryption test.\n");

    tool::Logging(myName.c_str(), "test done.\n");
    tool::Logging(myName.c_str(), "encryption (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / enc_time);
    tool::Logging(myName.c_str(), "hashing (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / hash_time);
    tool::Logging(myName.c_str(), "HMAC (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / hmac_time);
    tool::Logging(myName.c_str(), "decryption (MiB/s): %.3lf\n", (read_buffer_size * TEST_TIME) / MiB_2_B / dec_time);
    
    free(input_buffer);
    free(output_buffer);
    free(tmp_buffer);
    free(key);
    delete cipher;
    EVP_CIPHER_CTX_free(cipherCtx);
    EVP_MD_CTX_free(mdCtx);
    return 0;
}