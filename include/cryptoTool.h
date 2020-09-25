/**
 * @file crypto.h
 * @author Zuoru YANG (zryang@cse.cuhk.edu.hk)
 * @brief define the interface of crypto operations  
 * @version 0.1
 * @date 2020-09-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CRYPTO_TOOL_H
#define CRYPTO_TOOL_H
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include "define.h"
#include "dataStructure.h"
#include "configure.h" 

using namespace std;

static string ENCRYPT_STRING[] = {"AES_256_GCM", "AES_256_CFB", "AES_256_CBC"};
static string HASHER_STRING[] = {"SHA_1", "SHA_256"};

class CryptoTool {
private:
    EVP_CIPHER_CTX* ctx_;
    EVP_MD_CTX* mdCtx_;
    // the type of cipher
    ENCRYPT_SET cipherType_;
    // the type of hash 
    HASH_SET hashType_;

    // initialized vector
    uint8_t* iv_; 
public:
    /**
     * @brief Construct a new Crypto Tool object
     * 
     * @param cipherType the type of 
     * @param hashType 
     */
    CryptoTool(int cipherType, int hashType);

    /**
     * @brief Destroy the Crypto Tool object
     * 
     */
    ~CryptoTool();

    /**
     * @brief Generate the hash of the input data
     * 
     * @param inputData input data buffer 
     * @param dataSize input data size 
     * @param hash the output hash 
     * @return true success 
     * @return false fail
     */
    bool GenerateHash(uint8_t* inputData, const int dataSize, uint8_t* hash);

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
    bool EncryptWithKey(uint8_t* inputData, const int dataSize, uint8_t* key, uint8_t* cipherText);

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
    bool DecryptWithKey(uint8_t* cipherText, const int dataSize, uint8_t* key, uint8_t* dataBuffer);

};


#endif // !CRYPTO_TOOL_H