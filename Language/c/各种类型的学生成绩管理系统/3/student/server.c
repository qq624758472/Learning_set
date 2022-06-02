/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-04-06 21:00:02
 * @LastEditors: ruog__
 * @LastEditTime: 2022-04-08 17:17:33
 */
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
#include <time.h>
#include "server.h"

extern cmd ctl;

int max_fd(int a[], int n)
{
    int max = 0;
    for (int i = 0; i < n; i++)
    {
        if (max < a[i])
        {
            max = a[i];
        }
    }

    return max;
}

int N = 4;       //最大支持客户端个数
int port = 4399; //端口号。
int server_sock = 0;

int start()
{
#if 1
    //创建共享内存
    cmd *shareMemory = NULL;
    int shm_id = create_shm(&shareMemory);
    memset(shareMemory, 0, sizeof(cmd));

    if (N > FD_SETSIZE)
    {
        N = FD_SETSIZE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error\n");
        exit(-1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("bind error\n");
        return 0;
    }

    if (listen(server_sock, 5) == -1)
    {
        printf("listent error\n");
        exit(-1);
    }

    fd_set fd[2];
    FD_ZERO(&fd[0]);
    FD_SET(server_sock, &fd[0]);
    int sock[N];
    memset(sock, 0, sizeof(int) * N);
    sock[0] = server_sock;
    int count = 0;

    while (1)
    {
        struct timeval tv = {5, 0}; //超时时间。 设置为NULL 表示阻塞。
        FD_ZERO(&fd[1]);
        fd[1] = fd[0];                                                  // select 会清空没有响应的描述符。  所以需要备份一个。
        int ret = select(max_fd(sock, N) + 1, &fd[1], NULL, NULL, &tv); //用来等待文件描述词状态的改变。
        // int ret = select(max_fd(sock, N) + 1, &fd[1], NULL, NULL, NULL); //用来等待文件描述词状态的改变。 1：描述符可读事件。2.可写事件监听。
        if (ret < 0)
        {
            printf("select error\n");
        }
        else if (ret == 0)
        {
            // cout << "time out  ";
        }
        else
        {
            if (FD_ISSET(sock[0], &fd[1]) && count < N - 1)
            {
                struct sockaddr_in client_addr;
                memset(&client_addr, 0, sizeof(client_addr));
                unsigned int len = sizeof(client_addr);
                int new_sock = accept(sock[0], (struct sockaddr *)&client_addr, &len);
                if (new_sock == -1)
                {
                    printf("accept error\n");
                }
                else
                {
                    for (int i = 1; i < N; i++)
                    {
                        if (sock[i] == 0)
                        {
                            sock[i] = new_sock;
                            FD_SET(new_sock, &fd[0]);
                            count++;
                            printf("new connect success!\n");
                            break;
                        }
                    }
                }
            }
            char recvbuf[2048] = {0};
            char sendbuf[2048] = {0};
            // int nNetTimeout=1000;//1秒，设置接收超时时间。
            // setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
            for (int i = 1; i < N; i++)
            {
                if (FD_ISSET(sock[i], &fd[1]))
                {
                    int tmp = recv(sock[i], recvbuf, sizeof(recvbuf), 0);
                    if (tmp < 0)
                    {
                        printf("recv error\n");
                        perror("recv error code:");
                        FD_CLR(sock[i], &fd[0]);
                        close(sock[i]);
                        sock[i] = 0;
                        count--;
                        continue;
                    }
                    else if (tmp == 0)
                    {
                        /* code */
                        // perror("error code:");
                        printf("链接断开\n");
                        FD_CLR(sock[i], &fd[0]);
                        close(sock[i]);
                        sock[i] = 0;
                        count--;
                        continue;
                    }
                    else
                    {
                        //成功获取客户端信息，然后打印出来。
                        memset(shareMemory, 0, sizeof(cmd));
                        memcpy(shareMemory, recvbuf, 2048);
                        printf("socket:======start=====\n");
                        for(int i=0;i<16;i++)
                        {
                            if(i%16 == 0) printf("\n");
                            printf("s:%x  ", shareMemory->data[i]);
                        }
                        printf("socket:=======end======\n");
                    }
                }
            } // end for
        }
    } // end while
    return 1;
#else
    while (1)
    {
        sleep(1);
    }
#endif
}

int create_shm(cmd **shareMemory)
{
    int shmid = shmget((key_t)1234, sizeof(cmd), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        printf("shmget err.\n");
        return 0;
    }

    void *shm = shmat(shmid, (void *)0, 0);
    if (shm == (void *)-1)
    {
        printf("shmat err.\n");
        return 0;
    }

    *shareMemory = (cmd *)shm;
    return shmid;
}

int del_shm(cmd *shareMemory, int shmid)
{
    shmdt(shareMemory);
    shmctl(shmid, IPC_RMID, 0);
}

//判断全局变量的cmd命令是否不等于0， 等于0时循环检测。
//不等于0时，返回cmd值， 返回buff中的值。
int myscanf(cmd *shareMemory, char *buff, int isFloat)
{
    if(shareMemory == NULL)
    {
        printf("共享内存空间报错，程序异常\n");
        exit(0);
    }
    while (1)
    {
        if (shareMemory->cmdCtr != 0)
        {
            switch (shareMemory->cmdCtr)
            {
            case 1: //
                if (buff != NULL)
                {
                    if (isFloat == 1)
                    {
                        float tmp = atof(shareMemory->data);
                        printf("接收到客户端数据：%f\n", tmp);
                        memcpy(buff, &tmp, sizeof(float));                        
                        fflush(stdout);
                    }
                    else
                    {
                        if(strlen(shareMemory->data) == 1)
                        {
                            buff[0] = shareMemory->data[0];
                            printf("接收到客户端数据：%c\n", buff[0]);
                        }
                        else
                        {
                            memcpy(buff, shareMemory->data, strlen(shareMemory->data) == 1 ? 2 : strlen(shareMemory->data));
                            printf("接收到客户端数据：%s\n", buff);
                        }                        
                        
                        fflush(stdout);
                    }
                }
                int tmp = atoi(shareMemory->data);
                memset(shareMemory, 0, sizeof(cmd));
                // printf("接收到控制命令:%d\n", tmp);
                return tmp;
                break;

            default:
                break;
            }
        }
        usleep(5000);
    }
}