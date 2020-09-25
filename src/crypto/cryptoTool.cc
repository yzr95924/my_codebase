/**
 * @file crypto.c
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief  
 * @version 0.1
 * @date 2020-09-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../../include/cryptoTool.h"

/**
 * @brief Construct a new Crypto Tool object
 * 
 * @param cipherType the type of 
 * @param hashType 
 */
struct timeval startTime;
struct timeval endTime;

CryptoTool::CryptoTool(int cipherType, int hashType) {
    cipherType_ = static_cast<ENCRYPT_SET>(cipherType);
    hashType_ = static_cast<HASH_SET>(hashType);
    iv_ = (uint8_t*) malloc(sizeof(uint8_t) * CRYPTO_BLOCK_SIZE);
    if (!iv_) {
        fprintf(stderr, "CryptoTool: allocate the memory for iv fail.\n");
        exit(EXIT_FAILURE);
    }
    memset(iv_, 0, sizeof(uint8_t) * CRYPTO_BLOCK_SIZE);
    ctx_ = EVP_CIPHER_CTX_new();
    mdCtx_ = EVP_MD_CTX_new();

    if (!ctx_) {
        fprintf(stderr, "CryptoTool: Cannot initial cipher ctx.\n");
        exit(EXIT_FAILURE);
    }

    if (!mdCtx_) {
        fprintf(stderr, "CryptoTool: Cannot initial hash ctx.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "---------CryptoTool Configuration---------\n");
    fprintf(stderr, "openssl version: %s\n", OPENSSL_VERSION_TEXT);
    fprintf(stderr, "cipher type: %s\n", ENCRYPT_STRING[cipherType_].c_str());
    fprintf(stderr, "hasher type: %s\n", HASHER_STRING[hashType_].c_str());
    fprintf(stderr, "------------------------------------------\n");
}


/**
 * @brief Destroy the Crypto Tool object
 * 
 */
CryptoTool::~CryptoTool() {
    fprintf(stderr, "CryptoTool: Destory the object.\n");

    if (iv_) {
        free(iv_);
    }
    EVP_CIPHER_CTX_free(this->ctx_);
    EVP_MD_CTX_free(this->mdCtx_);
}

/**
 * @brief Encrypt the data with the encryption key
 * 
 * @param inputData input data buffer 
 * @param dataSize input data size 
 * @param key encryption key  
 * @param cipherText the output ciphertext 
 * @return true success 
 * @return false fail 
 */
bool CryptoTool::EncryptWithKey(uint8_t* inputData, const int dataSize, uint8_t* key, uint8_t* cipherText) {
    EVP_CIPHER_CTX_set_padding(this->ctx_, 0);
    int cipherLen = 0;
    int len = 0;

    switch (cipherType_) {
        case AES_256_CBC: 
            if (!EVP_EncryptInit_ex(this->ctx_, EVP_aes_256_cbc(), NULL,
                key, this->iv_)) {
                fprintf(stderr, "CryptoTool: Init error.\n");
                EVP_CIPHER_CTX_reset(this->ctx_);
                return false;
            }
            break;
        case AES_256_CFB:
            if (!EVP_EncryptInit_ex(this->ctx_, EVP_aes_256_cfb(), NULL,
                key, this->iv_)) {
                fprintf(stderr, "CryptoTool: Init error.\n");
                EVP_CIPHER_CTX_reset(this->ctx_);
                return false;
            }
            break;
        case AES_256_GCM:
            if (!EVP_EncryptInit_ex(this->ctx_, EVP_aes_256_gcm(), NULL,
                key, this->iv_)) {
                fprintf(stderr, "CryptoTool: Init error.\n");
                EVP_CIPHER_CTX_reset(this->ctx_);
                return false;
            }
            break;
    }

    if (!EVP_EncryptUpdate(this->ctx_, cipherText, &cipherLen, inputData, 
        dataSize)) {
        fprintf(stderr, "CryptoTool: Encryption error.\n");
        EVP_CIPHER_CTX_reset(this->ctx_);
        return false;
    }

    if (!EVP_EncryptFinal_ex(this->ctx_, cipherText + cipherLen, &len)) {
        fprintf(stderr, "CryptoTool: Encryption error.\n");
        EVP_CIPHER_CTX_reset(this->ctx_);
        return false;
    }

    cipherLen += len;

    if (cipherLen != dataSize) {
        EVP_CIPHER_CTX_reset(this->ctx_);
        fprintf(stderr, "CryptoTool: encryption output size not equal to origin size.\n");
        return false;
    }

    EVP_CIPHER_CTX_reset(this->ctx_);
    return true;
}


/**
 * @brief Decrypt the ciphertext with the encryption key  
 * 
 * @param cipherText ciphertext data buffer 
 * @param dataSize input data size 
 * @param key encryption key 
 * @param dataBuffer the output ciphertext 
 * @return true success 
 * @return false fail 
 */
bool CryptoTool::DecryptWithKey(uint8_t* cipherText, const int dataSize, uint8_t* key, uint8_t* dataBuffer) {
    int plainLen;
    int len;

    EVP_CIPHER_CTX_set_padding(this->ctx_, 0);

    switch (cipherType_) {
        case AES_256_CBC:
            if (!EVP_DecryptInit_ex(this->ctx_, EVP_aes_256_cbc(), NULL, 
                key, iv_)) {
                fprintf(stderr, "CryptoTool: Init error.\n");
                EVP_CIPHER_CTX_reset(this->ctx_);
                return false;
            }
            break;
        case AES_256_CFB:
            if (!EVP_DecryptInit_ex(this->ctx_, EVP_aes_256_cfb(), NULL,
                key, iv_)) {
                fprintf(stderr, "CryptoTool: Init error.\n");
                EVP_CIPHER_CTX_reset(this->ctx_);
                return false;
            }
            break;
        case AES_256_GCM:
            if (!EVP_DecryptInit_ex(this->ctx_, EVP_aes_256_gcm(), NULL,
                key, iv_)) {
                fprintf(stderr, "CryptoTool: Init error.\n");
                EVP_CIPHER_CTX_reset(this->ctx_);
                return false;
            }
            break;
    }

    if (!EVP_DecryptUpdate(this->ctx_, dataBuffer, &plainLen, cipherText, 
        dataSize)) {
        fprintf(stderr, "CryptoTool: Decrypt error.\n");
        EVP_CIPHER_CTX_reset(this->ctx_);
        return false;
    }

    if (!EVP_DecryptFinal_ex(this->ctx_, dataBuffer + plainLen, &len)) {
        fprintf(stderr, "CryptoTool: Decrypt error.\n");
        EVP_CIPHER_CTX_reset(this->ctx_);
        return false;
    }

    plainLen += len;

    if (plainLen != dataSize) {
        EVP_CIPHER_CTX_reset(this->ctx_);
        fprintf(stderr, "CryptoTool: Decrypt output size not equal to origin size.\n");
        return false;
    }

    EVP_CIPHER_CTX_reset(this->ctx_);
    return true;
}

/**
 * @brief Generate the hash of the data 
 * 
 * @param dataBuffer the data buffer  
 * @param dataSize input data size 
 * @param hash hash data buffer 
 * @return true success  
 * @return false fail 
 */
bool CryptoTool::GenerateHash(uint8_t* dataBuffer, const int dataSize, uint8_t* hash) {
    int expectedHashSize = 0;
    switch (hashType_) {
        case SHA_1:
            if (!EVP_DigestInit_ex(this->mdCtx_, EVP_sha1(), NULL)) {
                fprintf(stderr, "CryptoTool: Hash init error.\n");
                EVP_MD_CTX_reset(this->mdCtx_);
                return false;
            }
            expectedHashSize = 20;
            break;
        case SHA_256:
            if (!EVP_DigestInit_ex(this->mdCtx_, EVP_sha256(), NULL)) {
                fprintf(stderr, "CryptoTool: Hash init error.\n");
                EVP_MD_CTX_reset(this->mdCtx_);
                return false; 
            }
            expectedHashSize = 32;
            break;
        case MD5:
            if (!EVP_DigestInit_ex(this->mdCtx_, EVP_md5(), NULL)) {
                fprintf(stderr, "CryptoTool: Hash init error.\n");
                EVP_MD_CTX_reset(this->mdCtx_);
                return false;
            }
            expectedHashSize = 16;
            break;
    }
    if (!EVP_DigestUpdate(this->mdCtx_, dataBuffer, dataSize)) {
        fprintf(stderr, "CryptoTool: Hash error.\n");
        EVP_MD_CTX_reset(this->mdCtx_);
        return false;
    }
    uint32_t hashSize;
    if (!EVP_DigestFinal_ex(this->mdCtx_, hash, &hashSize)) {
        fprintf(stderr, "CryptoTool: Hash error.\n");
        EVP_MD_CTX_reset(this->mdCtx_);
        return false;
    }

    if (hashSize != expectedHashSize) {
        fprintf(stderr, "CryptoTool: Hash size error.\n");
        EVP_MD_CTX_reset(this->mdCtx_);
        return false;
    }

    EVP_MD_CTX_reset(this->mdCtx_);
    return true;
}