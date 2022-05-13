/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2022-02-25 19:48:58
 * @LastEditors: lsh
 * @LastEditTime: 2022-02-25 20:00:04
 */
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
typedef void (*init_call)(void);

/*
 * These two variables are defined in link script.
 */
extern init_call _init_start;
extern init_call _init_end;
#define _init __attribute__((unused, section(".myinit")))
#define DECLARE_INIT(func) init_call _fn_##func _init = func


static void C_init(void)
{
    printf("C_init\n");
}
DECLARE_INIT(C_init);