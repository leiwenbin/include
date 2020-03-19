#include <utils/openssl/evp.h>
#include <utils/openssl/pem.h>
#include <utils/openssl/aes.h>
#include <utils/openssl/err.h>
#include <utils/openssl/rand.h>

#include <stdio.h>
#include <string>
#include <string.h>

#ifndef CRYPTO_H
#define CRYPTO_H

#define RSA_KEYLEN 4096
#define AES_KEYLEN 256
#define AES_ROUNDS 6

#define SUCCESS 0
#define FAILURE -1

#define KEY_REMOTE_PUB  1
#define KEY_LOCAL_PUB   2
#define KEY_LOCAL_PRI   3
#define KEY_AES         4
#define KEY_AES_IV      5

class Crypto {

protected:
    typedef enum SecurityLevel {
        low = 0x01,
        high
    } SECURITY_LEVEL;

public:
    Crypto();

    ~Crypto();

    int AesEncrypt(const unsigned char* msg, size_t msgLen, unsigned char** encMsg);

    int AesEncrypt256(const unsigned char* msg, size_t msgLen, unsigned char** encMsg, SECURITY_LEVEL securityLevel = high);

    int AesDecrypt(unsigned char* encMsg, size_t encMsgLen, unsigned char** decMsg);

    int AesDecrypt256(unsigned char* encMsg, size_t encMsgLen, unsigned char** decMsg, SECURITY_LEVEL securityLevel = high);

    int WriteKeyToFile(FILE* fd, int key);

    int SetAesKey(unsigned char* key, size_t keyLen);

    int SetAesKey256(unsigned char* key, size_t keyLen, SECURITY_LEVEL securityLevel = high);

    int SetAesIv(unsigned char* iv, size_t ivLen);

    int SetAesIv256(unsigned char* iv, size_t ivLen, SECURITY_LEVEL securityLevel = high);

    int RsaEncrypt(const unsigned char* msg, size_t msgLen, unsigned char** encMsg);

    int RsaDecrypt(const unsigned char* encMsg, size_t encMsgLen, unsigned char** decMsg);

    int RsaSignByLocalPriKey(const unsigned char* text, unsigned char** signature);

    int RsaVerifyByLocalPubKey(const unsigned char* text, const unsigned char* signature);

    int RsaVerifyByRemotePubKey(const unsigned char* text, const unsigned char* signature);

    int SetRsaLocalPubKey(unsigned char* key, size_t keyLen);

    int SetRsaLocalPriKey(unsigned char* key, size_t keyLen);

    int SetRsaRemotePubKey(unsigned char* key, size_t keyLen);

    int GenerateAesIv();

    int InitAes();

    int InitAes256(SECURITY_LEVEL securityLevel = high);

    int InitRsa(unsigned int bits);

    void ShowRsa(unsigned char** pub, unsigned char** pri);

protected:

    void FreeAes();

    void FreeRsa();

private:
    EVP_CIPHER_CTX* aesEncryptCtx;
    EVP_CIPHER_CTX* aesDecryptCtx;

    unsigned char* aesKey;
    unsigned char* aesIv;

    RSA* rsaRemotePubKey;
    RSA* rsaLocalPubKey;
    RSA* rsaLocalPriKey;
    unsigned int bits;

    bool aesInitState;
};

#endif
