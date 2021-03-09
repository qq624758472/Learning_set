#include "common.h"
#include "pipe.h"

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


#ifdef DEBUG
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
    return 0;
}