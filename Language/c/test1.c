/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-08-16 17:23:35
 * @LastEditors: ruog__
 * @LastEditTime: 2023-08-16 17:31:09
 */
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("arg not enough.\nUsage: HW_INIT.elf [options] file...\nOptions:\n  1:external clock.\n  2:Internal Clock.\n");
        return -1;
    }

    int type = atoi(argv[1]);
    printf("type:%d\n", type);
}