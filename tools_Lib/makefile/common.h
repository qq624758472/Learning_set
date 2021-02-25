
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



//openssl文件夹
int test();
//call1文件夹
int test1();