#include "pipe.h"
#include <unistd.h>
#include <sys/stat.h>

using namespace std;
/*
 * 此文件为进程间通信，管道方式通信使用demo汇总。
*/

/*
//无名管道。若写入速度比读取速度快，
//再往里写就会提示resource temporarilly unvailable，甚至出现段错误。
在管道中的数据是通用的。 两边同时读写，造成数据混乱，所以叫半双工.
*/
int Pipe::pipe_use()
{
    char buf[8] = "";
    char buf1[8] = "";
    if(pipe(this->fifoFd)<0){
        cout << "pipe error!" << endl;
        return -1;
    }
    int pid = fork();
    if(pid < 0){
        cout <<"fork error !" << endl;
        return -1;
    }
    else if(pid > 0){ //返回子进程id. 
        while(1){
            memset(buf,0,8);
            read(this->fifoFd[0],buf , 7);  //若没有数据，就阻塞了。 
            cout << "read1: " << buf << endl;

            write(this->fifoFd[1],"2222222",strlen("22222222222"));
            sleep(1);
        }
    }
    else{  //=0 是子进程
        while(1){
            write(this->fifoFd[1],"1111111111",strlen("1111111111"));
            //write(this->fifoFd[1],"2222222222",strlen("2222222222"));

            memset(buf1,0,8);
            read(this->fifoFd[0],buf , 7);
            cout << "read2: " << buf << endl;
            sleep(1);
        }
    }
}



/*
*   有名管道：不相关的进程也能交换数据。 全双工。
*   O_NONBLOCK：非阻塞标志。 若没有此标志，只读open要阻塞到其他进程为写而打开它。
*               若指定，则只读open立即返回。若没有进程已经为只读而打开一个FIFO,那么只写open将返回-1，出错。
*/
int Pipe::pipe_name_use()
{
    mkfifo(fifoName , O_NONBLOCK); 
}