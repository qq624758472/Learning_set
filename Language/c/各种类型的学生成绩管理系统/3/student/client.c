/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-04-06 20:58:30
 * @LastEditors: ruog__
 * @LastEditTime: 2022-04-07 18:05:27
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
#include "server.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
    // define
    struct sockaddr_in addr_in;
    struct sockaddr_in addr_out;
    cmd ctl;
    memset(&ctl,0,sizeof(cmd));
    ctl.cmdCtr = 1;
    char *buffer = &ctl;

    // init
    memset(&addr_in, 0, sizeof(addr_in));

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("socket Error!\n");
        return -1;
    }

    int flag = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(4399);
    addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(struct sockaddr);

    if (connect(fd, (struct sockaddr *)&addr_in, len) == -1)
    {
        printf("connect Error!");
        return -1;
    }

    while (1)
    {   
        memset(&ctl.data, 0,sizeof(ctl.data));
        scanf("%s", &ctl.data);
        ctl.cmdCtr = 1;
        if (send(fd, buffer, strlen(buffer), 0) == -1)
        {
            printf("send buffer Error!");
            return -1;
        }
        printf("alerdy Send\n");
    }
}