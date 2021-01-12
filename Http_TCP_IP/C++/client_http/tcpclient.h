#ifndef _TCP_CLIENT_ 
#define _TCP_CLIENT_

#include <netinet/in.h>  
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stddef.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#include <string> 
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;


#define GETTOKEN "/api2/auth-token/"
#define SELF_IP "127.0.0.1"
#define SEAFILE_POINT 8000
#define True 1
#define False 0

#define BETAPIGDEBUG

typedef struct _tcpclient{ 
    int socket;
    int remote_port;     
    char remote_ip[16];  
    struct sockaddr_in _addr; 
    int connected;       
} tcpclient;

enum _Method
{
    GET = 0,
    POST
};


/**
 * \ingroup BetapigInterface
 *  创建一个 socket
 *
 * @Param tcpclient 上下文
 * @Param host 主机ip地址
 * @Param port 主机端口号
 *
 * @return   
 * 
 * @verbatim
////example: 
tcpclient_create(&client,"192.168.6.96",8000);
   @endverbatim
 */
int tcpclient_create(tcpclient *,const char *host, int port);

/**
 * \ingroup BetapigInterface
 *  链接服务器接口
 *
 * @Param tcpclient 上下文
 *
 * @return   
 */
int tcpclient_conn(tcpclient *);

/**
 * \ingroup BetapigInterface
 *  接收socket数据接口
 *
 * @Param tcpclient 上下文
 * @Param **lpbuff 接收数据的指针
 * @Param sizeof 接收数据的buf长度
 *
 * @return   
 */
int tcpclient_recv(tcpclient *,char **lpbuff,int size);

/**
 * \ingroup BetapigInterface
 *  发送socket数据接口
 *
 * @Param tcpclient 上下文
 * @Param buff 发送的数据buffer
 * @Param size 发送的数据长度
 *
 * @return   
 */
int tcpclient_send(tcpclient *,char *buff,int size);

/**
 * \ingroup BetapigInterface
 *  关闭socket套接字
 *
 * @Param tcpclient 上下文
 *
 * @return   
 */
int tcpclient_close(tcpclient *);

/**
 * \ingroup BetapigInterface
 *  设置发送的请求方式
 *
 * @Param post
 * @Param method
 * @Param page
 *
 * @return   
 * 
 * @verbatim
////example: 
http_set_method(post,method[POST], page);
http_set_method(post,method[GET], page);
  @endverbatim
 */
int http_set_method(char *post,char* method,char *page);

/**
 * \ingroup BetapigInterface
 *  设置请求头中的HOST字段放到host中。
 *  
 *  pclient : tcpclient 类型结构体，由tcpclient_create（）创建。
 *  @verbatim
////example: 
http_set_host(host , pclient);
    @endverbatim
 */
int http_set_host(char *host,tcpclient *pclient);

/**
 * \ingroup BetapigInterface
 *  设置自定义请求头
 *  
 *  key：value    键值对
 * @verbatim
////example: 
http_set_header(header, "Content-Type", "multipart/form-data; boundary=----123");
   @endverbatim
 */
int http_set_header(char *header, char * key, char *value);

/**
 * \ingroup BetapigInterface
 *  设置Content-Length请求头，放到content_len中。
 *  
 *  body ：http body内容
 */
int http_set_content_len(char *content_len,char *body);

/**
 * \ingroup BetapigInterface
 *  设置同账号登陆接口的body
 *  
 *  body ：
 *  my_method ： check or login
 *  username : 
 *  passwd  :
 *  mac    :   
////example: 
http_same_account_set_body(body,my_method, username, passwd, mac);
 */
int http_same_account_set_body(char *body,char *my_method, char *username, char*passwd, char *mac);

/**
 * \ingroup BetapigInterface
 *  设置seafile登陆接口的body
 *  
 *  body ：
 *  username : 
 *  passwd  :
////example: 
http_login_set_body(body, username, passwd);
 */
int http_login_set_body(char *body, char *username, char*passwd);

/**
 * \ingroup BetapigInterface
 *  seafile登陆接口
 *  
 *  pclient: 创建的socket客户端
 *  page ：  url                  /api2/auth-token/
 *  username :     
 *  passwd  :
 *  response : 服务器返回的信息
////example: 
login(&client,“/api2/auth-token/”, "homecloud@saming.com", "123456",&response)；
 */
int login(tcpclient *pclient,char *page, char *username, char*passwd,char **response);

/**
 * \ingroup BetapigInterface
 *  同账号登陆的登陆接口
 *  
 *  pclient: 创建的socket客户端
 *  page ：  url                  /api2/auth-token/
 *  my_method ： check or login
 *  username :     
 *  passwd  :
 *  mac : 客户端mac地址
 *  response : 服务器返回的信息
////example: 
sameAccountLogin(&client,“/api2/auth-token/”, "homecloud@saming.com", "123456",&response)；
 */
int sameAccountLogin(tcpclient *pclient,char *page,char *my_method, char *username, char*passwd, char *mac,char **response);
/**
 * \ingroup BetapigInterface
 *  从http接收的buff中查找头为key的值
 *  
////example: 
char *buf = {
            "POST http://192.168.6.96:8000/api2/auth-token/ HTTP/1.1\r\n"
            "Host: 192.168.6.96:8000\r\n"
            "Content-Type:multipart/form-data; boundary=----123\r\n"
            "Content-Length: 167\r\n\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"username\"\r\n\r\n"
            "homecloud@saming.com\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"password\"\r\n\r\n"
            "123456\r\n"
            "------123--\r\n"};
    char *key = "Content-Type";
    char *value = http_get_header(buf, key);
 */
char * http_get_header(char * buf, char * key);


/**
 * \ingroup BetapigInterface
 *  从接收到的buff中获取username和password的值。
 * 
 *  返回value
 *  
////example: 
    http_get_from_body(char*buf,char * username);
 */
char * http_get_from_body(char* buf, char *key);

/**
 * \ingroup BetapigInterface
 *  从接收到的buff中获取url的值。
 * 
 *  返回url的值。
 *  
////example: 
    http_get_url(char*buf);
 */
char * http_get_url(char * buf);

/**
 * \ingroup BetapigInterface
 *  从接收到的buff中获取mathod的值。
 * 
 *  返回mathod的值。
 *  
////example: 
    http_get_method(char*buf);
 */
char * http_get_method(char * buf);

/**
 * \ingroup BetapigInterface
 *  格式化接收到的字符串。
 * 
 *  返回格式化后的结构体。
 *  
////example: 
    make_str(char*buf);
 */
char* make_str(char * buf );

/**
 * \ingroup BetapigInterface
 *  将buffer内的值写入日志，测试用。
 * 
 *  
////example: 
    write_file(char*buf);
 */
int write_file(char *buffer);




/**
 * \ingroup Trans Server Define
 *  链接服务器接口。
 *  
 *  pclient: 创建的socket客户端
 *  lpbuf  :  组装好的http请求buffer.
 *  response : 服务器返回的信息
////example: 
connected_server(pclient , lpbuf, response);
 */
int connected_server(tcpclient *pclient , char * lpbuf,char **response);




/**
 * \ingroup Trans Server Define
 *  获取资料库信息接口。
 *  
 *  page: 访问的URL
 *  username  :  
 *  passwd :
 *  response : 服务器返回的信息
////example: 
connected_server(pclient , lpbuf, response);

http:
GET http://192.168.6.96:8000/api2/repos/ HTTP/1.1
Host: 192.168.6.96:8000
Authorization: Token f1a91624361592e6aa9e28ae5fecf02bc010496f
 */
int getDataLibId(tcpclient *pclient,char *page, char *username, char*passwd,char **response);


char * js_get_value_for_key(char*buf, char* key);
int forward(tcpclient *pclient,char *page, char* token,char **response);





/**
 * \ingroup Trans Server Define
 *  解析JS的类
 *  
////example: 
connected_server(pclient , lpbuf, response);
 */
class AnalyzeJS
{
    public:
        char *buff = NULL;

    public:
        AnalyzeJS(int len)
        {
            buff = (char*) malloc(len * sizeof(char));
            memset(buff, '\0', len);
        }        

        char *get_value_for_key(char*buff, char* key, char* value);

};




#endif
