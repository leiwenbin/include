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

public:
    Crypto();

    ~Crypto();

    int AesEncrypt(const unsigned char* msg, size_t msgLen, unsigned char** encMsg);

    int AesDecrypt(unsigned char* encMsg, size_t encMsgLen, unsigned char** decMsg);

    int WriteKeyToFile(FILE* fd, int key);

    int SetAesKey(unsigned char* aesKey, size_t aesKeyLen);

    int SetAesIv(unsigned char* aesIv, size_t aesIvLen);

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
};

#endif
