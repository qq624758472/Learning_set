#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>  //sleep

using namespace std;

#define MAX_FD 10

struct List_client{
	int client_id;
	int client_fd;
	List_client *next;
};


void add_client_list(void * client_list, int fd);
int if_fd_exist(int fd, void * client_list);
int add_fd(int fd, void * client_list);
void show_all_client(void *p_client);

