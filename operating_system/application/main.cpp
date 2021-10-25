/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-04-26 11:18:59
 * @LastEditors: lsh
 * @LastEditTime: 2021-10-20 16:50:16
 */
#include "common.h"
#include "pipe.h"
#include "thread.h"
#include <iostream>

using namespace std;

#define DEBUG

int main(int argc,char *argv[])
{
    Pipe test;
#ifdef DEBUG1
    test.pipe_use();
#endif

#ifdef DEBUG2
    //管道没有加密数据。
    if(strncmp(argv[1],"server",6) ==0)
    {
        test.pipe_name_use();
    }
    else if(strncmp(argv[1],"client",6) ==0)
    {
        test.pipe_name_client();
    }
    else
    {
        cout << "ERROR" << endl;
    }
#endif


#ifdef DEBUG3
    //管道使用加密数据传输。
    if(strncmp(argv[1],"server",6) ==0)
    {
        test.en_pipe_name_use();
    }
    else if(strncmp(argv[1],"client",6) ==0)
    {
        //test.pipe_name_client();
        test.de_pipe_name_client();
    }
    else
    {
        cout << "ERROR" << endl;
    }
#endif


#ifdef DEBUG
    JiShi lll;
    pause();
#endif
    return 0;
}