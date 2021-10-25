/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-10-20 16:42:53
 * @LastEditors: lsh
 * @LastEditTime: 2021-10-20 16:44:44
 */

#include "thread.h"
#include "common.h"
JiShi::JiShi()
{
    thread_create();
}

void *thread1(void* data)
{
    for(int i=0;i<10;i++)
    {
        printf ("111111111111\n");
    }
}



void JiShi::thread_create(void)   //创建线程
{
    pthread_t thread;  //两个线程
    int temp;
    memset(&thread, 0, sizeof(thread));          //comment1
    /*创建线程*/
    if((temp = pthread_create(&thread, NULL, thread1, NULL)) != 0)  //comment2
            printf("线程1创建失败!/n");
    else
            printf("线程1被创建/n");
    pthread_join(thread,NULL);
}