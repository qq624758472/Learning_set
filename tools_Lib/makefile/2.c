#include"common.h"




int main(int argc, char *argv[])
{
    hmackey_t key;
    char skey[sizeof(hmackey_t) * 2 + 1];

    {
        uint8_t u[513] = "hhh";
        uint8_t r[128] = "uosice";
        uint8_t p[257] = "123456";

        // STRCPY(u, "hhh");
        // STRCPY(r, "uosice");
        // STRCPY(p, "123456");

        //size_t sz = get_hmackey_size(SHATYPE_DEFAULT);
        //size_t sz;
        stun_produce_integrity_key_str(u, r, p, key, SHATYPE_DEFAULT, skey);

        printf("\n===============\n%s\n", skey);
    }

    test();
    return 0;
}