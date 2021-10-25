/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-10-20 16:42:48
 * @LastEditors: lsh
 * @LastEditTime: 2021-10-20 16:44:15
 */
#ifndef JISHI_H
#define JISHI_H

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

class JiShi
{
public:
    JiShi();

public:
    void thread_create(void);
};

#endif // JISHI_H
