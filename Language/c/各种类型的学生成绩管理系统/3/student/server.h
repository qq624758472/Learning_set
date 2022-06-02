/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-04-07 10:03:12
 * @LastEditors: ruog__
 * @LastEditTime: 2022-04-07 17:45:50
 */

#ifndef SERVER__
#define SERVER__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>



typedef struct _cmd
{
    /* data */
    char cmdCtr;
    char data[2047];
}cmd;


//判断全局变量的cmd命令是否不等于0， 等于0时循环检测。 
//不等于0时，返回cmd值， 返回buff中的值。
int myscanf(cmd *shareMemory, char *buff, int isFloat);
int del_shm(cmd *shareMemory, int shmid);
int create_shm(cmd **shareMemory);
#endif