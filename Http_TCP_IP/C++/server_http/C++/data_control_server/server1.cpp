
#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <cstring>
#include <fstream>

//视频流服务器， 打开视频，并推流。

using namespace std;

#define DEFAULT_PORT 4399



std::ofstream __log;



void http_request_cb(struct evhttp_request *req, void *arg);
void http_request_openvideo(struct evhttp_request *req, void *arg);
int main(int argc, char **argv) 
{
    int port = DEFAULT_PORT;
    if (argc == 2) 
    {
        port = atoi(argv[1]);
    }
    
    struct event_base *base = event_base_new();
    
    struct evhttp *http = evhttp_new(base);
    evhttp_set_gencb(http, http_request_cb, NULL);
    evhttp_set_cb(http, "/openvideo", http_request_openvideo, NULL);

    evhttp_bind_socket(http, "0.0.0.0", port);
    
    event_base_dispatch(base);
    
    return 0;
}

void http_request_openvideo(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    FILE * fp = NULL;
    __log.open("openvideo");
    char buffer[10240] = "openvideo success";
    //获取发送来的信息。
    evkey_input = evhttp_request_get_input_headers(req);
    const char *username = evhttp_find_header(evkey_input,"username");
    const char *alarm = evhttp_find_header(evkey_input,"alarm");

    __log << buffer << std::endl;
    __log.close();

access_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, HTTP_OK, "OK", evb); 
    evbuffer_free(evb);
    printf("FU QIn\n");
    return;
error_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","error");
    evhttp_send_reply(req, 500, "STATERR", evb); 
    evbuffer_free(evb);
    return;
}


void http_request_cb(struct evhttp_request *req, void *arg) 
{
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    FILE * fp = NULL;
    __log.open("alarm.log");
    char buffer[10240] = "0";
    //获取发送来的信息。
    evkey_input = evhttp_request_get_input_headers(req);
    const char *username = evhttp_find_header(evkey_input,"username");
    const char *alarm = evhttp_find_header(evkey_input,"alarm");
    cout << "username:" << username << endl;
    cout << "alarm:" << alarm << endl;
    if(username == NULL){
        memcpy(buffer,"username is NULL",strlen("username is NULL"));
        goto error_response;
    }
    if(alarm == NULL){
        memcpy(buffer,"alarm is NULL",strlen("alarm is NULL"));
        goto error_response;
    }
    memcpy(buffer,username,strlen(username));
    memcpy(buffer+strlen(username),alarm,strlen(alarm));


    if(0 == access("openvideo", F_OK)){ //存在
        memset(buffer,0,sizeof(buffer));
        memcpy(buffer,"openvideo",strlen("openvideo"));
        if(remove("openvideo") != 0){
            system("rm /home/openvideo");
        }
	}
    
    __log << buffer << std::endl;
    __log.close();

access_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, HTTP_OK, "OK", evb); 
    evbuffer_free(evb);
    printf("FU QIn\n");
    return;
error_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","error");
    evhttp_send_reply(req, 500, "STATERR", evb); 
    evbuffer_free(evb);
    return;
}
