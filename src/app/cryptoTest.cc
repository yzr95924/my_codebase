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

#include "../../include/crypto/crypto_util.h"

const string my_name = "CryptoTest";

void Usage() {
    fprintf(stderr, "%s -i [input file path] -c [cipher type] -h [hash type] -s [chunk size (KiB)] "
        "-c [cipher type]:\n"
        "\tAES-256-GCM: 0\n"
        "\tAES-128-GCM: 1\n"
        "\tAES-256-CFB: 2\n"
        "\tAES-128-CFB: 3\n"
        "\tAES-256-CBC: 4\n"
        "\tAES-128-CBC: 5\n"
        "\tAES-256-ECB: 6\n"
        "\tAES-128-ECB: 7\n"
        "\tAES-256-CTR: 8\n"
        "\tAES-128-CTR: 9\n"
        "-h [hash type]:\n"
        "\tSHA-256: 0\n"
        "\tMD5: 1\n"
        "\tSHA-1: 2\n", my_name.c_str());
    return ;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    const char opt_string[] = "i:c:h:s:";  
    int option;
    uint32_t chunk_size = 0;
    ENCRYPT_SET cipher_type;
    HASH_SET hash_type;
    string input_path;

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
            case 'c': {
                switch (atoi(optarg)) {
                    case AES_256_GCM: {
                        cipher_type = AES_256_GCM;
                        tool::Logging(my_name.c_str(), "using AES-256-GCM.\n");
                        break;
                    }
                    case AES_128_GCM: {
                        cipher_type = AES_128_GCM;
                        tool::Logging(my_name.c_str(), "using AES-128-GCM.\n");
                        break;
                    }
                    case AES_256_CFB: {
                        cipher_type = AES_256_CFB;
                        tool::Logging(my_name.c_str(), "using AES-256-CFB.\n");
                        break;
                    }
                    case AES_128_CFB: {
                        cipher_type = AES_128_CFB;
                        tool::Logging(my_name.c_str(), "using AES-128-CFB.\n");
                        break;
                    }
                    case AES_256_CBC: {
                        cipher_type = AES_256_CBC;
                        tool::Logging(my_name.c_str(), "using AES-256-CBC.\n");
                        break;
                    }
                    case AES_128_CBC: {
                        cipher_type = AES_128_CBC;
                        tool::Logging(my_name.c_str(), "using AES-128-CBC.\n");
                        break;
                    }
                    case AES_256_ECB: {
                        cipher_type = AES_256_ECB;
                        tool::Logging(my_name.c_str(), "using AES-256-ECB.\n");
                        break;
                    }
                    case AES_128_ECB: {
                        cipher_type = AES_128_ECB;
                        tool::Logging(my_name.c_str(), "using AES-128-ECB.\n");
                        break;
                    }
                    case AES_256_CTR: {
                        cipher_type = AES_256_CTR;
                        tool::Logging(my_name.c_str(), "using AES-256-CTR.\n");
                        break;
                    }
                    case AES_128_CTR: {
                        cipher_type = AES_128_CTR;
                        tool::Logging(my_name.c_str(), "using AES-128-CTR.\n");
                        break;
                    }
                    default: {
                        tool::Logging(my_name.c_str(), "wrong cipher type.\n");
                        Usage();
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            }
            case 'h': {
                switch (atoi(optarg)) {
                    case SHA_256: {
                        hash_type = SHA_256;
                        tool::Logging(my_name.c_str(), "using SHA-256.\n");
                        break;;
                    }
                    case MD5: {
                        hash_type = MD5;
                        tool::Logging(my_name.c_str(), "using MD5.\n");
                        break;
                    }
                    case SHA_1: {
                        hash_type = SHA_1;
                        tool::Logging(my_name.c_str(), "using SHA-1.\n");
                        break;
                    }
                    default: {
                        tool::Logging(my_name.c_str(), "wrong hash type.\n");
                        Usage();
                        exit(EXIT_FAILURE);
                    }
                }
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
            case '?':
                tool::Logging(my_name.c_str(), "error optopt: %c\n", optopt);
                tool::Logging(my_name.c_str(), "error opterr: %d\n", opterr);
                Usage();
                exit(EXIT_FAILURE);
        }
    }

    CryptoUtil* crypto_util = new CryptoUtil(cipher_type, hash_type);

    uint8_t input_chunk[MAX_CHUNK_SIZE] = {0};
    uint8_t enc_chunk[MAX_CHUNK_SIZE + CRYPTO_BLOCK_SIZE] = {0};
    uint8_t dec_chunk[MAX_CHUNK_SIZE] = {0};

    EVP_CIPHER_CTX* cipher_ctx = EVP_CIPHER_CTX_new();
    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    uint8_t key[CHUNK_HASH_SIZE] = {0};
    memset(key, 2, CHUNK_HASH_SIZE);
    uint8_t hash[CHUNK_HASH_SIZE] = {0};
    uint8_t iv[CRYPTO_BLOCK_SIZE] = {0};
    memset(iv, 1, CRYPTO_BLOCK_SIZE);

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
    double enc_time = 0;
    double dec_time = 0;
    double hash_time = 0;

    uint32_t enc_chunk_size = 0;
    uint32_t dec_chunk_size = 0;
    uint64_t total_enc_data_size = 0; 
    while (!is_end) {
        input_file_hdl.read((char*)input_chunk, chunk_size);
        read_byte_num = input_file_hdl.gcount();
        is_end = input_file_hdl.eof();
        if (read_byte_num == 0) {
            break;
        }

        total_enc_data_size += read_byte_num;

        gettimeofday(&stime, NULL);
        enc_chunk_size = crypto_util->EncryptWithKeyIV(cipher_ctx, input_chunk, 
            read_byte_num, key, iv, enc_chunk);
        gettimeofday(&etime, NULL);
        enc_time += tool::GetTimeDiff(stime, etime);

        gettimeofday(&stime, NULL);
        dec_chunk_size = crypto_util->DecryptWithKeyIV(cipher_ctx, enc_chunk,
            enc_chunk_size, key, iv, dec_chunk);
        gettimeofday(&etime, NULL);
        dec_time += tool::GetTimeDiff(stime, etime);

        if (correct_test) {
            // check the size first
            tool::Logging(my_name.c_str(), "read byte size: %lu\n", read_byte_num);
            if (dec_chunk_size != read_byte_num) {
                tool::Logging(my_name.c_str(), "dec length is not correct.\n");
                exit(EXIT_FAILURE);
            } else {
                if (memcmp(input_chunk, dec_chunk, read_byte_num) != 0) {
                    tool::Logging(my_name.c_str(), "the dec content is wrong.\n");
                    exit(EXIT_FAILURE);
                }
            }
            correct_test = false;
            tool::Logging(my_name.c_str(), "correctness test is passed.\n");
        }

        gettimeofday(&stime, NULL);
        crypto_util->GenerateHash(md_ctx, input_chunk, read_byte_num, hash);
        gettimeofday(&etime, NULL);
        hash_time += tool::GetTimeDiff(stime, etime);

        if (is_end) {
            break;
        }
    }

    fprintf(stdout, "--------Test Result--------\n");
    fprintf(stdout, "enc speed (MiB/s): %lf\n", static_cast<double>(total_enc_data_size) / 
        1024.0 / 1024.0 / enc_time);
    fprintf(stdout, "dec speed (MiB/s): %lf\n", static_cast<double>(total_enc_data_size) / 
        1024.0 / 1024.0 / dec_time);
    fprintf(stdout, "hash speed (MiB/s): %lf\n", static_cast<double>(total_enc_data_size) / 
        1024.0 / 1024.0 / hash_time);

    delete crypto_util;
    EVP_CIPHER_CTX_free(cipher_ctx);
    EVP_MD_CTX_free(md_ctx);
    return 0;
}