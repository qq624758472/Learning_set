
#include "server.h"
 
int main()
{
	//define
	struct sockaddr_in addr_in;
	struct sockaddr_in addr_out;
	socklen_t len = 0;
	char buffer[1024] = "0";
	char if_continue = 0;
	char enter = 0;
	fd_set rset;
	int fd_new = -1;
	List_client client;
	List_client * p_client = &client;
	char buf_read[64] = "0";

	//init
	memset(p_client,0,sizeof(List_client));
	p_client->next = NULL;
	memset(&addr_in,0, sizeof(addr_in));
	memset(&addr_out,0, sizeof(addr_in));
	memset(buf_read,0, sizeof(buf_read));

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		printf("socket Error!\n");
		return -1;
	}

	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(8080);
	addr_in.sin_addr.s_addr = INADDR_ANY;
	if(bind(fd, (struct sockaddr *)&addr_in, sizeof(addr_in)) == -1) {
		printf("bind Error!\n");
		return -1;
	}
	cout << "Start listen " << endl;

	if(listen(fd,10) == -1) {
		cout << "listen Error!" << endl;
		return -1;
	}
	
	int fd_number = 0;
	while(1) {
		FD_ZERO(&rset);
		FD_SET(fd,&rset);
		FD_SET(STDIN_FILENO,&rset);

		fd_number  = 0;
		fd_number = select(fd+1, &rset,NULL,NULL,NULL);
		if(fd_number == -1) {
			cout << "select Error!" << endl;
			return -1;
		}else if(fd_number == 0) {
			cout << "NOT fd be ready! " << endl;
			//continue;
		}
		cout << "----------" << endl;
		if(FD_ISSET(fd, &rset)){
			cout << "fd exist !" << endl;
			cout << "listen end ,Start accept " << endl;
			fd_new = accept(fd, (struct sockaddr *)&addr_out, &len);
			if(fd_new  == -1) {
				cout << "accept Error!" << endl;
				return -1;
			}
			FD_SET(fd_new,&rset);//add new 
			add_client_list(p_client, fd_new);
		}
		cout << "------2----" << endl;

		if(FD_ISSET(fd_new, &rset)){
			memset(buffer,0,1024);
			if(recv(fd_new, buffer, 1024, 0) == -1) {
				cout << "recv Error!" << endl;
				return -1;
			}
			cout << "buffer value is:[" << buffer << "]" << endl;
		}

		if(FD_ISSET(STDIN_FILENO, &rset)){ //监测到用户输入
			read(STDIN_FILENO, buf_read, sizeof(buf_read));
			if(strncmp(buf_read,"cat client list",15) == 0)
			{
				show_all_client(p_client);
			}
		}
	}
}


void show_all_client(void *p_client)
{
	//define 
	List_client *p = (List_client *)p_client;

	//init 

	//program
	if(p->next == NULL)
	{
		if(p->client_fd == 0)
		{ //empty list
			cout << "------client list is empty---------" << endl;
		}
		else 
		{ //exist one node
			cout << "-------------------------------------------" << endl;
			cout << "-----" << "client_id = " << p->client_id << "--------"<< endl;
			cout << "-----" << "client_fd = " << p->client_fd << "--------"<< endl;
		}
		goto success;
	}

	while(1)
	{

		cout << "-------------------------------------------" << endl;
		cout << "-----" << "client_id = " << p->client_id << "--------"<< endl;
		cout << "-----" << "client_fd = " << p->client_fd << "--------"<< endl;
		p = p->next;
		if(p == NULL)
		{
			goto success;
		}
	}
	
	//return
success:
	return;
}

/*
给客户端列表加入新客户端,新客户端存在，则不需要加，否则加入列表。
return :
	null
*/
void add_client_list(void * client_list, int fd)
{
	
	if(if_fd_exist(fd,client_list) == -1){ 
		add_fd(fd,client_list);
	}
}



/*
遍历线性表，判断fd是否在列表中
return :
	@-1:   not exist
	@!=-1: exist
*/
int if_fd_exist(int fd, void * client_list) 
{
	//define
	List_client * p_head = (List_client *)client_list;
	List_client * p = p_head->next;

	//init 

	//program
	if(p == NULL){
		if(p_head->client_fd == 0){ //empty list
			goto not_exist;
		}
		else { //exist one node
			if(p_head->client_fd == fd){ 
				goto exist;
			}
			else {
				goto not_exist;
			}
		}
	}

	while(1) 
	{
		if(p_head->client_fd == fd)
		{
			goto exist;
		}
		else 
		{
			p_head = p_head->next;
			if(p_head == NULL)
			{
				goto not_exist;
			}
			else
			{
				continue;
			}
		}
	}


	//return
not_exist:
	return -1;
exist:
	return 1;
error:
	return 0;
}




/*
遍历线性表，将fd插入到列表中
return :
	@-1:   error
	@!=-1: success
*/
int add_fd(int fd, void * client_list) 
{
	//define
	List_client * p_head = (List_client *)client_list;
	List_client * p = NULL;
	int id = 0;

	//init 


	//program
	if(p_head->next == NULL)
	{
		if(p_head->client_fd == 0)
		{ //empty list
			p_head->client_id = id;
			p_head->client_fd = fd;
		}
		else
		{ //exist one node
			p = new List_client;
			if(p == NULL)
			{
				cout << "new Error! function:" << __FUNCTION__  << "line:" << __LINE__ <<endl;
				goto error;
			}

			p_head->next = p;
			p_head->client_id = id + 1;
			p_head->client_fd = fd;
		}
		goto success;
	}

	while(1) 
	{
		if(p_head->next == NULL)
		{
			p = new List_client;
			if(p == NULL)
			{
				cout << "new Error! function:" << __FUNCTION__  << "line:" << __LINE__ <<endl;
				goto error;
			}
			p_head->next = p;
			p_head->client_id = id + 1;
			p_head->client_fd = fd;
			goto success;
		}
		p_head = p_head->next;
		id++;
	}

	//return
success:
	return 1;
error:
	return -1;
}