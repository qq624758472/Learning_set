/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-01-29 10:05:39
 * @LastEditors: lsh
 * @LastEditTime: 2022-02-10 16:56:40
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[])
{
    // char buff[4096] = {0};
    int sockfd, n;
    char recvline[4096] = {0}, sendline[4096] = {0};
    struct sockaddr_in servaddr;

    char *buf = "POST /logon\r\n\
    content-type: application/x-www-form-urlencoded\r\n\
    cache-control: no-cache\r\n\
    user-agent: PostmanRuntime/7.1.1\r\n\
    accept: */*\r\n\
    host: 121.36.4.162:8080\r\n\
    accept-encoding: gzip, deflate\r\n\
    content-length: 20\r\n\r\n\
    uname=lshpwd=123456\r\n";

    char *send_str = {
        "POST http://121.36.4.162:8080/logon/　HTTP/1.1\r\n"
        "Host: 121.36.4.162:8080\r\n"
        "Content-Type:application/x-www-form-urlencoded\r\n"
        "cache-control: no-cache\r\n"
        "accept: */*\r\n"
        "accept-encoding: gzip, deflate\r\n"
        "Content-Length: 22\r\n\r\n"
        "uname=lsh&&pwd=123456\r\n"};

    if (argc != 2)
    {
        printf("usage: ./client <ipaddress>\n");
        return 0;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", argv[1]);
        return 0;
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    printf("send msg to server.\n");
    // fgets(sendline, 4096, stdin);
    if (send(sockfd, send_str, strlen(send_str), 0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    char tmp = 0;
    cout << "buff:" << endl;
    while (1)
    {
        // tmp = 0;
        memset(recvline, 0, sizeof(recvline));
        recv(sockfd, recvline, 4096, 0);
        cout << recvline << endl;
    }

    close(sockfd);
    return 0;
}
