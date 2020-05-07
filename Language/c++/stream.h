
#ifndef STREAM
#define STREAM

#include "common.h"

#define PATH ("./test.log")

class FileOperating{
    private:
        int data1 = 0;
        int data2 = 0;
        char *buff = NULL;

    public:
        FileOperating(int a=0, int b=0) : data1(a),data2(b){}
        
        virtual int operating() = 0;
};

class FileRead : public FileOperating {

    public:
        int operating();

};

class FileWrite : public FileOperating {

    public:
        int operating();

};




#endif