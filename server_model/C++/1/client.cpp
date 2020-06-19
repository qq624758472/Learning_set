#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>  //sleep

using namespace std;

int main()
{
	//define
	struct sockaddr_in addr_in;
	struct sockaddr_in addr_out;
	char *buffer = "liushihao\n";

	//init
	memset(&addr_in,0, sizeof(addr_in));

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		printf("socket Error!\n");
		return -1;
	}

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(8080);
	addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(struct sockaddr);

	if(connect(fd, (struct sockaddr *)&addr_in, len) == -1) {
		cout << "connect Error!" << endl;
		return -1;
	}

	while(1) {
		if(	send(fd, buffer, strlen(buffer),0) == -1) {
			cout << "send buffer Error!" << endl;
			return -1;
		}
		cout << "alerdy Send" << endl;
		sleep(2);
	}
	
	

}
