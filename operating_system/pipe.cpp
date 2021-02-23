#include "pipe.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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
    const char *fifo_name = "/tmp/my_fifo";
    int pipe_fd = -1;
    int data_fd = -1;
    int res = 0;
    const int open_mode = O_WRONLY;
    int bytes_sent = 0;
    char buffer[PIPE_BUF + 1];
    int bytes_read = 0;

    if(access(fifo_name, F_OK) == -1)
    {
        printf ("Create the fifo pipe.\n");
        res = mkfifo(fifo_name, 0777);

        if(res != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);
            exit(EXIT_FAILURE);
        }
    }

    printf("Process %d opening FIFO O_WRONLY\n", getpid());
    pipe_fd = open(fifo_name, open_mode);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if(pipe_fd != -1)
    {
        bytes_read = 0;
        data_fd = open("Data.txt", O_RDONLY);
        if (data_fd == -1)
        {
            close(pipe_fd);
            fprintf (stderr, "Open file[Data.txt] failed\n");
            return -1;
        }

        bytes_read = read(data_fd, buffer, PIPE_BUF);
        buffer[bytes_read] = '\0';
        while(bytes_read > 0)
        {

            res = write(pipe_fd, buffer, bytes_read);
            if(res == -1)
            {
                fprintf(stderr, "Write error on pipe\n");
                exit(EXIT_FAILURE);
            }

            bytes_sent += res;
            bytes_read = read(data_fd, buffer, PIPE_BUF);
            buffer[bytes_read] = '\0';
        }

        close(pipe_fd);
        close(data_fd);
    }
    else
        exit(EXIT_FAILURE);

    printf("Process %d finished\n", getpid());
    exit(EXIT_SUCCESS);
}