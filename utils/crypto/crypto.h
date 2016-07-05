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

#define RSA_KEYLEN 2048
#define AES_KEYLEN 256
#define AES_ROUNDS 6

//#define PSUEDO_CLIENT

//#define USE_PBKDF

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2
#define KEY_AES        3
#define KEY_AES_IV     4

class Crypto {

public:
    Crypto();

    Crypto(unsigned char* remotePubKey, size_t remotePubKeyLen);

    ~Crypto();

    int InitRsa();

    int InitAes();

    int RsaEncrypt(const unsigned char* msg, size_t msgLen, unsigned char** encMsg, unsigned char** ek, size_t* ekl, unsigned char** iv, size_t* ivl);

    int AesEncrypt(const unsigned char* msg, size_t msgLen, unsigned char** encMsg);

    int RsaDecrypt(unsigned char* encMsg, size_t encMsgLen, unsigned char* ek, size_t ekl, unsigned char* iv, size_t ivl, unsigned char** decMsg);

    int AesDecrypt(unsigned char* encMsg, size_t encMsgLen, unsigned char** decMsg);

    int WriteKeyToFile(FILE* fd, int key);

    int GetRemotePubKey(unsigned char** pubKey);

    int SetRemotePubKey(unsigned char* pubKey, size_t pubKeyLen);

    int GetLocalPubKey(unsigned char** pubKey);

    int GetLocalPriKey(unsigned char** priKey);

    int GetAESKey(unsigned char** aesKey);

    int SetAESKey(unsigned char* aesKey, size_t aesKeyLen);

    int GetAESIv(unsigned char** aesIv);

    int SetAESIv(unsigned char* aesIv, size_t aesIvLen);

private:
    EVP_PKEY* localKeypair;
    EVP_PKEY* remotePubKey;

    EVP_CIPHER_CTX* rsaEncryptCtx;
    EVP_CIPHER_CTX* aesEncryptCtx;

    EVP_CIPHER_CTX* rsaDecryptCtx;
    EVP_CIPHER_CTX* aesDecryptCtx;

    unsigned char* aesKey;
    unsigned char* aesIV;

    int GenTestClientKey();
};

#endif
