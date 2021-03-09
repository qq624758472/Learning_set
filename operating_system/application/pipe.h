#ifndef PIPE
#define PIPE

#include <fstream>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>


#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h> 

using namespace std;




class Pipe{


public:
    char * fifoName = "/home/fifoTest1";
    int fifoFd[2] = {-1,-1};

public:
    //
    int pipe_use();

    int pipe_name_use();
    int pipe_name_client();

    int en_pipe_name_use();     //加密管道数据
    int de_pipe_name_client();  //解密管道数据
};


#endif