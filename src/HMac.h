#pragma once

#include <openssl/hmac.h>
#include <openssl/evp.h>

unsigned char* hmac(const char* algorithm, unsigned char* keyBytes, int keyLength, unsigned char* text, size_t textLength, unsigned int* lengthOut) {
    HMAC_CTX* ctx = HMAC_CTX_new();

    const EVP_MD* digest = EVP_get_digestbyname(algorithm);
    if (!digest)
        return NULL;

    HMAC_Init_ex(ctx, keyBytes, keyLength, digest, NULL);
    HMAC_Update(ctx, text, textLength);

    unsigned char* result = malloc(EVP_MAX_MD_SIZE * sizeof(unsigned char));
    unsigned int resultLen;
    HMAC_Final(ctx, result, &resultLen);

    HMAC_CTX_free(ctx);

    *lengthOut = resultLen;
    return result;
}
