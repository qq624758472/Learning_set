
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/ossl_typ.h>

#define UNUSED_ARG(A) \
    do                \
    {                 \
        A = A;        \
    } while (0)

enum _SHATYPE
{
    SHATYPE_ERROR = -1,
    SHATYPE_DEFAULT = 0,
    SHATYPE_SHA1 = SHATYPE_DEFAULT,
    SHATYPE_SHA256,
    SHATYPE_SHA384,
    SHATYPE_SHA512
};
#define __SIZE_TYPE__ long unsigned int
typedef __SIZE_TYPE__ size_t;

typedef unsigned char __uint8_t;
typedef __uint8_t uint8_t;
typedef uint8_t hmackey_t[64];

typedef enum _SHATYPE SHATYPE;

int stun_produce_integrity_key_str(const uint8_t *uname, const uint8_t *realm, const uint8_t *upwd, hmackey_t key, SHATYPE shatype, char skey[129]);


int stun_produce_integrity_key_str(const uint8_t *uname, const uint8_t *realm, const uint8_t *upwd, hmackey_t key, SHATYPE shatype, char skey[129])
{
    int ret;

    //get_hmackey_size()函数封装
    size_t szo = 16;
    if (shatype == SHATYPE_SHA256)
        szo = 32;
    if (shatype == SHATYPE_SHA384)
        szo = 48;
    if (shatype == SHATYPE_SHA512)
        szo = 64;

    ERR_clear_error();
    UNUSED_ARG(shatype);

    size_t ulen = strlen((const char *)uname);
    size_t rlen = strlen((const char *)realm);
    size_t plen = strlen((const char *)upwd);
    size_t sz = ulen + 1 + rlen + 1 + plen + 1 + 10;
    size_t strl = ulen + 1 + rlen + 1 + plen;
    uint8_t *str = (uint8_t *)malloc(sz + 1);

    strncpy((char *)str, (const char *)uname, sz);
    str[ulen] = ':';
    strncpy((char *)str + ulen + 1, (const char *)realm, sz - ulen - 1);
    str[ulen + 1 + rlen] = ':';
    strncpy((char *)str + ulen + 1 + rlen + 1, (const char *)upwd, sz - ulen - 1 - rlen - 1);
    str[strl] = 0;

    if (shatype == SHATYPE_SHA256)
    {
#if !defined(OPENSSL_NO_SHA256) && defined(SHA256_DIGEST_LENGTH)
#if OPENSSL_VERSION_NUMBER < 0x10100000L

        unsigned int keylen = 0;
        EVP_MD_CTX ctx;
        EVP_DigestInit(&ctx, EVP_sha256());
        EVP_DigestUpdate(&ctx, str, strl);
        EVP_DigestFinal(&ctx, key, &keylen);
        EVP_MD_CTX_cleanup(&ctx);
#else

        unsigned int keylen = 0;
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        EVP_DigestInit(ctx, EVP_sha256());
        EVP_DigestUpdate(ctx, str, strl);
        EVP_DigestFinal(ctx, key, &keylen);
        EVP_MD_CTX_free(ctx);
#endif
        ret = 0;
#else

        fprintf(stderr, "SHA256 is not supported\n");
        ret = -1;
#endif
    }
    else if (shatype == SHATYPE_SHA384)
    {
#if !defined(OPENSSL_NO_SHA384) && defined(SHA384_DIGEST_LENGTH)
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        unsigned int keylen = 0;
        EVP_MD_CTX ctx;
        EVP_DigestInit(&ctx, EVP_sha384());
        EVP_DigestUpdate(&ctx, str, strl);
        EVP_DigestFinal(&ctx, key, &keylen);
        EVP_MD_CTX_cleanup(&ctx);
#else
        unsigned int keylen = 0;
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        EVP_DigestInit(ctx, EVP_sha384());
        EVP_DigestUpdate(ctx, str, strl);
        EVP_DigestFinal(ctx, key, &keylen);
        EVP_MD_CTX_free(ctx);
#endif
        ret = 0;
#else
        fprintf(stderr, "SHA384 is not supported\n");
        ret = -1;
#endif
    }
    else if (shatype == SHATYPE_SHA512)
    {
#if !defined(OPENSSL_NO_SHA512) && defined(SHA512_DIGEST_LENGTH)
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        unsigned int keylen = 0;
        EVP_MD_CTX ctx;
        EVP_DigestInit(&ctx, EVP_sha512());
        EVP_DigestUpdate(&ctx, str, strl);
        EVP_DigestFinal(&ctx, key, &keylen);
        EVP_MD_CTX_cleanup(&ctx);
#else
        unsigned int keylen = 0;
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        EVP_DigestInit(ctx, EVP_sha512());
        EVP_DigestUpdate(ctx, str, strl);
        EVP_DigestFinal(ctx, key, &keylen);
        EVP_MD_CTX_free(ctx);
#endif
        ret = 0;
#else
        fprintf(stderr, "SHA512 is not supported\n");
        ret = -1;
#endif
    }
    else
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        //#if OPENSSL_VERSION_NUMBER < 0x110000000L
        unsigned int keylen = 0;
        EVP_MD_CTX ctx;
        EVP_MD_CTX_init(&ctx);
#if defined EVP_MD_CTX_FLAG_NON_FIPS_ALLOW && !defined(LIBRESSL_VERSION_NUMBER)
        if (FIPS_mode())
        {
            EVP_MD_CTX_set_flags(&ctx, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
        }
#endif
        EVP_DigestInit_ex(&ctx, EVP_md5(), NULL);
        EVP_DigestUpdate(&ctx, str, strl);
        EVP_DigestFinal(&ctx, key, &keylen);
        EVP_MD_CTX_cleanup(&ctx);
#else
        unsigned int keylen = 0;
        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
#if defined EVP_MD_CTX_FLAG_NON_FIPS_ALLOW && !defined(LIBRESSL_VERSION_NUMBER)
        if (FIPS_mode())
        {
            EVP_MD_CTX_set_flags(ctx, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
        }
#endif
        EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
        EVP_DigestUpdate(ctx, str, strl);
        EVP_DigestFinal(ctx, key, &keylen);
        EVP_MD_CTX_free(ctx);
#endif
        ret = 0;
    }

    free(str);

    //算出skey的值
    size_t i = 0, j = 0;

    int maxsz = (int)(szo * 2) + 1;
    char *s = skey;
    for (i = 0; (i < szo) && (maxsz > 2); i++)
    {
        snprintf(s, (size_t)(szo * 2), "%02x", (unsigned int)key[i]);
        maxsz -= 2;
        s += 2;
    }
    skey[szo * 2] = 0;
    // char *t = key;
    // for (j = 0; j < 64; j++)
    // {
    //     printf("%d     ", *t);
    //     t++;
    // }

    return ret;
}