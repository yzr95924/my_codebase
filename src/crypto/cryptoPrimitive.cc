#include "../../include/cryptoPrimitive.h"

/**
 * @brief Construct a new Crypto Primitive object
 * 
 * @param cipherType 
 * @param hashType 
 */
CryptoPrimitive::CryptoPrimitive(int cipherType, int hashType) {
    // for openssl optimization
    OPENSSL_init_crypto(0, NULL);

    cipherType_ = static_cast<ENCRYPT_SET>(cipherType);
    hashType_ = static_cast<HASH_SET>(hashType);
    iv_ = (uint8_t*) malloc(sizeof(uint8_t) * CRYPTO_BLOCK_SIZE);
    if (!iv_) {
        tool::Logging(myName_.c_str(), "allocate the memory for iv fail.\n");
        exit(EXIT_FAILURE);
    }
    memset(iv_, 0, sizeof(uint8_t) * CRYPTO_BLOCK_SIZE);

    string deriveStr = "password";
    pKey_ = EVP_PKEY_new_raw_private_key(EVP_PKEY_HMAC, NULL, (uint8_t*)deriveStr.c_str(),
        deriveStr.size());
}

/**
 * @brief Destroy the Crypto Primitive object
 * 
 */
CryptoPrimitive::~CryptoPrimitive() {
    free(iv_);
    EVP_PKEY_free(pKey_);
}

/**
 * @brief Generate the hash of the input data
 * 
 * @param mdCtx hasher ctx
 * @param dataBuffer input data buffer
 * @param dataSize input data size 
 * @param hash output hash 
 */
void CryptoPrimitive::GenerateHash(EVP_MD_CTX* mdCtx, uint8_t* dataBuffer, const int dataSize, uint8_t* hash) {
    int expectedHashSize = 0;
    switch (hashType_) {
        case SHA_1:
            if (!EVP_DigestInit_ex(mdCtx, EVP_sha1(), NULL)) {
                tool::Logging(myName_.c_str(), "hash init error.\n");
                exit(EXIT_FAILURE);
            }
            expectedHashSize = 20;
            break;
        case SHA_256:
            if (!EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL)) {
                tool::Logging(myName_.c_str(), "hash init error.\n");
                exit(EXIT_FAILURE);
            }
            expectedHashSize = 32;
            break;
        case MD5:
            if (!EVP_DigestInit_ex(mdCtx, EVP_md5(), NULL)) {
                tool::Logging(myName_.c_str(), "hash init error.\n");
                exit(EXIT_FAILURE);
            }
            expectedHashSize = 16;
            break;
    }

    if (!EVP_DigestUpdate(mdCtx, dataBuffer, dataSize)) {
        tool::Logging(myName_.c_str(), "hash error.\n");
        exit(EXIT_FAILURE);
    }
    uint32_t hashSize;
    if (!EVP_DigestFinal_ex(mdCtx, hash, &hashSize)) {
        tool::Logging(myName_.c_str(), "hash error.\n");
        exit(EXIT_FAILURE);
    }

    if (hashSize != expectedHashSize) {
        tool::Logging(myName_.c_str(), "hash size error.\n");
        exit(EXIT_FAILURE);
    }
    
    EVP_MD_CTX_reset(mdCtx);
    return ;
}

/**
 * @brief Encrypt the data with the encryption key 
 * 
 * @param ctx cipher ctx
 * @param dataBuffer input data buffer
 * @param dataSize input data size 
 * @param key encryption key 
 * @param ciphertext output cipherText 
 */
void CryptoPrimitive::EncryptWithKey(EVP_CIPHER_CTX* ctx, uint8_t* dataBuffer, const int dataSize, 
    uint8_t* key, uint8_t* ciphertext) {
    int cipherLen = 0;
    int len = 0;

    switch (cipherType_) {
        case AES_128_CFB: {
            if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cfb(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_256_CFB: {
            if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_256_GCM: {
            EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, CRYPTO_BLOCK_SIZE, NULL);
            if (!EVP_EncryptInit_ex(ctx, NULL, NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_EncryptUpdate(ctx, NULL, &cipherLen, gcm_aad, sizeof(gcm_aad));
            break;
        }
        case AES_128_GCM: {
            EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, CRYPTO_BLOCK_SIZE, NULL);
            if (!EVP_EncryptInit_ex(ctx, NULL, NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_EncryptUpdate(ctx, NULL, &cipherLen, gcm_aad, sizeof(gcm_aad));
            break;
        }
        case AES_256_CTR: {
            if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            } 
            break;
        }
        case AES_128_CTR: {
            if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_256_ECB: {
            if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL,
                key, NULL)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_CIPHER_CTX_set_padding(ctx, 0);
            break;
        }
        case AES_128_ECB: {
            if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL,
                key, NULL)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_CIPHER_CTX_set_padding(ctx, 0);
            break;
        }
        default: {
            tool::Logging(myName_.c_str(), "wrong encryption type.\n");
            exit(EXIT_FAILURE);
        }
    }

    // encrypt the plaintext
    if (!EVP_EncryptUpdate(ctx, ciphertext, &cipherLen, dataBuffer, 
        dataSize)) {
        tool::Logging(myName_.c_str(), "encryption error.\n");
        exit(EXIT_FAILURE);
    }
    EVP_EncryptFinal_ex(ctx, ciphertext + cipherLen, &len);
    cipherLen += len;
	
    if (cipherLen != dataSize) {
        tool::Logging(myName_.c_str(), "encryption output size not equal to origin size"
            "cipherLen %d, len %d\n", cipherLen, dataSize);
        exit(EXIT_FAILURE);
    }

    EVP_CIPHER_CTX_reset(ctx);
    return ;
}

/**
 * @brief Decrypt the ciphertext with the encryption key
 * 
 * @param ctx cipher ctx
 * @param ciphertext ciphertext data buffer
 * @param dataSize input data size
 * @param key encryption key 
 * @param dataBuffer output ciphertext 
 */
void CryptoPrimitive::DecryptWithKey(EVP_CIPHER_CTX* ctx, uint8_t* ciphertext, const int dataSize, 
    uint8_t* key, uint8_t* dataBuffer) {
    int plainLen;
    int len;
    switch (cipherType_) {
        case AES_128_CFB: {
            if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, 
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_256_CFB: {
            if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_128_GCM: {
            EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, CRYPTO_BLOCK_SIZE, NULL);
            if (!EVP_DecryptInit_ex(ctx, NULL, NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_DecryptUpdate(ctx, NULL, &plainLen, gcm_aad, sizeof(gcm_aad));
            break;
        }
        case AES_256_GCM: {
            EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
            EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN, CRYPTO_BLOCK_SIZE, NULL);
            if (!EVP_DecryptInit_ex(ctx, NULL, NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_DecryptUpdate(ctx, NULL, &plainLen, gcm_aad, sizeof(gcm_aad));
            break;
        }
        case AES_256_CTR: {
            if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_128_CTR: {
            if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL,
                key, iv_)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case AES_256_ECB: {
            if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL,
                key, NULL)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_CIPHER_CTX_set_padding(ctx, 0);
            break;
        }
        case AES_128_ECB: {
            if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL,
                key, NULL)) {
                tool::Logging(myName_.c_str(), "init error.\n");
                exit(EXIT_FAILURE);
            }
            EVP_CIPHER_CTX_set_padding(ctx, 0);
            break;
        }
        default: {
            tool::Logging(myName_.c_str(), "wrong decryption type.\n");
            exit(EXIT_FAILURE);
        }
    }

    // decrypt the plaintext
    if (!EVP_DecryptUpdate(ctx, dataBuffer, &plainLen, ciphertext, 
        dataSize)) {
        tool::Logging(myName_.c_str(), "decrypt error.\n");
        exit(EXIT_FAILURE);
    }

    EVP_DecryptFinal_ex(ctx, dataBuffer + plainLen, &len);
    
    plainLen += len;

    if (plainLen != dataSize) {
        tool::Logging(myName_.c_str(), "decrypt output size not equal to origin size.\n");
        exit(EXIT_FAILURE);
    }

    EVP_CIPHER_CTX_reset(ctx);
    return ;
}

/**
 * @brief Generate the HMAC of the input data
 * 
 * @param mdCtx the mdCtx
 * @param inputData input data buffer
 * @param dataSize input data size
 * @param hMAC the output HMAC
 */
void CryptoPrimitive::GenerateHMAC(EVP_MD_CTX* mdCtx, uint8_t* inputData, 
    const int dataSize, uint8_t* hMAC) {
    int expectedHashSize = 0;
    switch (hashType_) {
        case SHA_1:
            if (EVP_DigestSignInit(mdCtx, NULL, EVP_sha1(), NULL, pKey_) != 1) {
                tool::Logging(myName_.c_str(), "HMAC init error, error 0x%lx\n", ERR_get_error());
                exit(EXIT_FAILURE);
            }
            expectedHashSize = 20;
            break;
        case SHA_256:
            if (EVP_DigestSignInit(mdCtx, NULL, EVP_sha256(), NULL, pKey_) != 1) {
                tool::Logging(myName_.c_str(), "HMAC init error, error 0x%lx\n", ERR_get_error());
                exit(EXIT_FAILURE);
            }
            expectedHashSize = 32;
            break;
        case MD5:
            if (EVP_DigestSignInit(mdCtx, NULL, EVP_md5(), NULL, pKey_) != 1) {
                tool::Logging(myName_.c_str(), "HMAC init error, error 0x%lx\n", ERR_get_error());
                exit(EXIT_FAILURE);
            }
            expectedHashSize = 16;
            break;
    }

    if (EVP_DigestSignUpdate(mdCtx, inputData, dataSize) != 1) {
        tool::Logging(myName_.c_str(), "HMAC update error, error 0x%lx\n", ERR_get_error());
        exit(EXIT_FAILURE);
    }

    // first call the buffer should be NULL, and receive the size of the signature
    size_t sigLen = CHUNK_HASH_SIZE;
    if (EVP_DigestSignFinal(mdCtx, hMAC, &sigLen) != 1) {
        tool::Logging(myName_.c_str(), "HMAC final error, error 0x%lx\n", ERR_get_error());
        exit(EXIT_FAILURE);
    }

    if (sigLen != expectedHashSize) {
        tool::Logging(myName_.c_str(), "HMAC sig len cannot match the expected size.\n");
        exit(EXIT_FAILURE);
    }

    EVP_MD_CTX_reset(mdCtx);

    return ;
}