#ifndef PIPE
#define PIPE

#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;




class Pipe{


public:
    char * fifoName = "/home/fifoTest1";
    int fifoFd[2] = {-1,-1};

public:
    //
    int pipe_use();

    int pipe_name_use();
};


#endif