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

//#define PSUEDO_CLIENT

//#define USE_PBKDF

#define SUCCESS 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2
#define KEY_CLIENT_PRI 3
#define KEY_LOCALE_PUB 4
#define KEY_LOCALE_PRI 5
#define KEY_AES        6
#define KEY_AES_IV     7

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

    int GetRemotePriKey(unsigned char** priKey);

    int SetRemotePriKey(unsigned char* priKey, size_t priKeyLen);

    int GetLocalPubKey(unsigned char** pubKey);

    int SetLocalPubKey(unsigned char* pubKey, size_t pubKeyLen);

    int GetLocalPriKey(unsigned char** priKey);

    int SetLocalPriKey(unsigned char* priKey, size_t priKeyLen);

    int GetAESKey(unsigned char** aesKey);

    int SetAESKey(unsigned char* aesKey, size_t aesKeyLen);

    int GetAESIv(unsigned char** aesIv);

    int SetAESIv(unsigned char* aesIv, size_t aesIvLen);

    int GenTestClientKey();

    int GenTestServerKey();

    int RsaEncryptNew(unsigned char* pubKey, size_t pubKeyLen, const unsigned char* msg, size_t msgLen, unsigned char** encMsg);

    int RsaDecryptNew(unsigned char* priKey, size_t priKeyLen, const unsigned char* encMsg, size_t encMsgLen, unsigned char** decMsg);

private:
    EVP_PKEY* localKeypair;
    EVP_PKEY* remoteKeypair;

    EVP_CIPHER_CTX* rsaEncryptCtx;
    EVP_CIPHER_CTX* aesEncryptCtx;

    EVP_CIPHER_CTX* rsaDecryptCtx;
    EVP_CIPHER_CTX* aesDecryptCtx;

    unsigned char* aesKey;
    unsigned char* aesIV;

    RSA* rsaLocalePubKey;
    RSA* rsaLocalePriKey;
};

#endif
