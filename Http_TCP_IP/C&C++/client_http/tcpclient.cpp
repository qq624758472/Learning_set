


#include "tcpclient.h"

#define BUFFER_SIZE 1024
#define METHOD_SIZE 5
#define URL_SIZE 1024
#define HOST_SIZE 32
#define CONTENT_LEN_SIZE 64
#define CONTENT_TYPE_SIZE 256
#define TMP_BUFF_SIZE 4096


char *method[8] = {
    "GET",
    "POST"
    };

typedef struct _PostInfo{
    char method[METHOD_SIZE];
    char url[URL_SIZE];
    char host[HOST_SIZE];
    char content_len[CONTENT_LEN_SIZE];
    char content_type[CONTENT_TYPE_SIZE];
}PostInfo;

int tcpclient_create(tcpclient *pclient,const char *host, int port)
{
    //define
    int ret = True;
    struct hostent *he;

    //if
    if(pclient == NULL || host == NULL || port == 0) 
    {
        printf("%s %d False!",__FUNCTION__, __LINE__);
        return False;
    }

    //init
    memset(pclient,'\0',sizeof(tcpclient));

    //program
    do{    
        if((he = gethostbyname(host))==NULL){
            ret = False;
            break;
        }

        pclient->remote_port = port;
        strcpy(pclient->remote_ip,inet_ntoa( *((struct in_addr *)he->h_addr) ));

        pclient->_addr.sin_family = AF_INET;
        pclient->_addr.sin_port = htons(pclient->remote_port);
        pclient->_addr.sin_addr = *((struct in_addr *)he->h_addr);

        if((pclient->socket = socket(AF_INET,SOCK_STREAM,0))==-1){
            ret = False;
            break;
        }
        
        // //套接字设置为非阻塞。
        // int flags = fcntl(pclient->socket, F_GETFL,0);
        // fcntl(pclient->socket,F_SETFL, flags| O_NONBLOCK);

    }while(0);

    return ret;
}

int tcpclient_conn(tcpclient *pclient)
{
    //define
    int ret = True;

    //if
    if(pclient == NULL){
        printf("%s %d False!",__FUNCTION__, __LINE__);
        return False;
    }

    do{
        if(pclient->connected){
            break;
        }

        if(connect(pclient->socket, (struct sockaddr *)&pclient->_addr,sizeof(struct sockaddr))==-1)
        {
            perror("connect: ");
            printf("%s %d False!\n",__FUNCTION__, __LINE__);
            ret = False;
            break;
        }
        pclient->connected = 1;
    }while(0);

    return ret;
}

int tcpclient_recv(tcpclient *pclient,char **lpbuff,int size)
{
    //if
    if(pclient == NULL || size < 0){
        printf("%s %d False!",__FUNCTION__, __LINE__);
        return False;
    }
    
    //define
    int recvnum=0,tmpres=0;
    char buff[BUFFER_SIZE] = "\0";

    *lpbuff = NULL;
    do{
        while(recvnum < size || size==0)
        {
            tmpres = recv(pclient->socket, buff,BUFFER_SIZE,0);
            if(tmpres < 0)
            {
                perror("recv: ");
                cout << "Error " << __FUNCTION__ << "  " << __LINE__ << endl;
                break;
            }
            if(tmpres == 0)
            {
                perror("recv: ");
                cout << "Error " << __FUNCTION__ << "  " << __LINE__ << endl;
                //pclient->connected = 0;
                break;
            }
            recvnum += tmpres;

            if(*lpbuff == NULL)
            {
                *lpbuff = (char*)malloc(recvnum);
                if(*lpbuff == NULL){
                    cout << "Error " << __FUNCTION__ << "  " << __LINE__ << endl;
                    recvnum = False;
                    break;
                }  
            }
            else
            {
                *lpbuff = (char*)realloc(*lpbuff,recvnum);  //改变内存大小。
                if(*lpbuff == NULL)
                {
                    printf("%s %d False!",__FUNCTION__, __LINE__);
                    recvnum = False;
                    break;
                }
            }

            memcpy(*lpbuff+recvnum-tmpres,buff,tmpres);
            //tcpclient_close(pclient);
        }         
    }while(0);
    

    return recvnum;
}

int tcpclient_send(tcpclient *pclient,char *buff,int size)
{
    //if
    if(pclient == NULL || buff == NULL || size < 0){
        printf("%s %d False!",__FUNCTION__, __LINE__);
        return False;
    }
    //define
    int sent=0,tmpres=0, ret = True;

    //init
    do{
        //program
        while(sent < size)
        {
            tmpres = send(pclient->socket,buff+sent,size-sent,0);
            if(tmpres < 0){
                printf("%s %d False!",__FUNCTION__, __LINE__);
                ret = False;
                break;
            }
            else if(tmpres == 0)
            {
                printf("%s %d False!",__FUNCTION__, __LINE__);
                ret = False;
                break;
            }
            sent += tmpres;
        }
    }while(0);
    return sent;
}

int tcpclient_close(tcpclient *pclient)
{
    close(pclient->socket);
    memset(pclient, '\0', sizeof(tcpclient));
    pclient->connected = 0;
}

int http_set_method(char *post,char* method,char *page)
{ 
    int ret = True;
    do{
        if(method == NULL || page == NULL || post == NULL)
        {
            printf("method page post is NULL\n");
            ret = False;
            break;
        }
        sprintf(post,"%s %s HTTP/1.1\r\n",method,page);
    }while(0);
    return True;
}
                                                               
int http_set_host(char *host,tcpclient *pclient)
{
    int ret = True;
    do{
        if(host == NULL || pclient == NULL)
        {
            printf("host pclient is NULL\n");
            ret = False;
            break;
        }
        sprintf(host,"Host: %s:%d\r\n",pclient->remote_ip,pclient->remote_port);
    }while(0);
    return True;
}

int http_set_header(char *header, char * key, char *value)
{
    int ret = True;
    do{
        if(header == NULL || key == NULL || value == NULL)
        {
            printf("header key value is NULL\n");
            ret = False;
            break;
        }

        char buffer[1024] = "";

        memset(buffer,0,sizeof(buffer));

        sprintf(buffer,"%s:%s\r\n",key, value);

        strcat(header, buffer);
    }while(0);
    return True;
}

int http_set_content_len(char *content_len,char *body)
{
    int ret = True;
    do{
        if(content_len == NULL || body == NULL)
        {
            printf("content_len body is NULL\n");
            ret = False;
            break;
        }
        sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(body)-strlen("\r\n"));
    }while(0);
    return True;
}

int http_same_account_set_body(char *body,char *my_method, char *username, char*passwd, char *mac)
{
    int ret = True;
    do{
        if(body == NULL || my_method == NULL || username == NULL || passwd == NULL ||  mac == NULL)
        {
            printf("body is NULL \n");
            ret = False;
            break;
        }
        sprintf(body,"------123\r\n"
            "Content-Disposition: form-data; name=\"method\"\r\n\r\n"
            "%s\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"username\"\r\n\r\n"
            "%s\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"passwd\"\r\n\r\n"
            "%s\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"mac\"\r\n\r\n"
            "%s\r\n"
            "------123--\r\n",my_method, username, passwd, mac);
    }while(0);

    return True;
}

int http_login_set_body(char *body, char *username, char*passwd)
{
    int ret = True;

    do{
        if(body == NULL || username == NULL || passwd == NULL)
        {
            printf("body is NULL \n");
            ret = False;
            break;
        }
        sprintf(body,"\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"username\"\r\n\r\n"
            "%s\r\n"
            "------123\r\n"
            "Content-Disposition: form-data; name=\"password\"\r\n\r\n"
            "%s\r\n"
            "------123--\r\n", username, passwd);
    }while(0);

    return ret;
}

//截取buf到下一个\r\n为止。
char * _find_string(char *buf, char * diff)
{
    char *p = buf;
    int len = strlen(buf);
    int i = 0;
    char str[TMP_BUFF_SIZE];

    memset(str,'\0',sizeof(str));

    for(i=0; i<len; i++)
    {
        //if(p[i] == diff && p[i+1] == '\n')
        if(memcmp(&p[i], diff, strlen(diff)) == 0)
        {
            break;
        }
        str[i] = p[i];
    }

    char * buff = (char *)malloc(sizeof(char) * (strlen(str)+1));
    memset(buff, '\0', (strlen(str)+1));
    memcpy(buff, str, strlen(str));
    
    return buff;
}

char * http_get_from_body(char* buf, char *key)
{
    if(buf == NULL || key == NULL)
    {
        printf("%s %d \n",__FUNCTION__, __LINE__);
        return NULL;
    }

    char *value = strstr(buf, key);
    if(value == NULL)
    {
        return NULL;
    }
    value = value + strlen(key) +3;
    char *buff = _find_string(value,"\"");
    return buff;
}


char * http_get_url(char * buf)
{
    char *key = "POST"; // error
    if(buf == NULL || key == NULL)
    {
        printf("%s %d \n",__FUNCTION__, __LINE__);
        return NULL;
    }
    char *value = strstr(buf, key);
    if(value == NULL)
    {
        return NULL;
    }
    value = value + strlen(key) +1;
    char *buff = _find_string(value,"HTTP/1.1\r\n");
    return buff;
}


char * http_get_header(char * buf, char * key)
{
    if(buf == NULL || key == NULL)
    {
        printf("%s %d False!",__FUNCTION__, __LINE__);
        return NULL;
    }
    char *value = strstr(buf, key);
    if(value == NULL)
    {
        //printf("%s %d False!",__FUNCTION__, __LINE__);
        return NULL;
    }
    value = value + strlen(key) +2;
    char *buff = _find_string(value, "\r\n");
    return buff;
}

char* make_str(char * buf )
{
    if(buf == NULL)
    {
        printf("%s %d\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    return buf;
}

char * http_get_method(char * buf)
{
    if(buf == NULL)
    {
        printf("%s %d \n",__FUNCTION__, __LINE__);
        return NULL;
    }
    
    char *buff = (char*)malloc((4 + 1) * sizeof(char));
    if(buff == NULL)
    {
        printf("%s %d \n",__FUNCTION__, __LINE__);
        goto error;
    }
    memset(buff,0, sizeof(buff));
    memcpy(buff, buf, 4);

success:
    return buff;
error:
    return NULL;
}


int connected_server(tcpclient *pclient , char * llpbuf,char **response, int len)
{
    //define
    int ret = True;
    char post[1024] = "";
    char *ptmp = NULL;
    char * lpbuf = NULL;

    do{
        if(!pclient->connected)
        {
            tcpclient_conn(pclient);
        }

        if(!tcpclient_send(pclient,llpbuf,len-1)){
            printf("tcpclient_send Error!");
            ret = False;
            break;
        }
        printf("发送请求:\n%s\n",llpbuf);
        cout << "------发送完成-------" <<endl;

        /*释放内存*/
        if(lpbuf != NULL) free(lpbuf);
        lpbuf = NULL;

        /*it's time to recv from server*/
        if(tcpclient_recv(pclient,&lpbuf,0) <= 0)
        {
            if(lpbuf) free(lpbuf);
            cout << __FUNCTION__ << "   " << __LINE__ << endl;
            ret = False;
            break;
        }
        printf("接收响应:\n%s\n",lpbuf);

        /*响应代码,|HTTP/1.0 200 OK|
        *从第10个字符开始,第3位
        * */
        memset(post,0,sizeof(post));
        strncpy(post,lpbuf+9,3);
        if(atoi(post)!=200){
            if(lpbuf) free(lpbuf);
            //return atoi(post);
            ret = False;
            break;
        }

        ptmp = (char*)strstr(lpbuf,"\r\n\r\n");
        if(ptmp == NULL){
            free(lpbuf);
            ret = False;
            break;
        }
        ptmp += 4;/*跳过\r\n*/

        len = strlen(ptmp)+1;
        *response=(char*)malloc(len);
        if(*response == NULL){
            if(lpbuf) free(lpbuf);
            ret = False;
            break;
        }
        memset(*response,0,len);
        memcpy(*response,ptmp,len-1);

        /*从头域找到内容长度,如果没有找到则不处理*/
        ptmp = (char*)strstr(lpbuf,"Content-Length:");
        if(ptmp != NULL){
            char *ptmp2;
            ptmp += 15;
            ptmp2 = (char*)strstr(ptmp,"\r\n");
            if(ptmp2 != NULL){
                memset(post,0,sizeof(post));
                strncpy(post,ptmp,ptmp2-ptmp);
                if(atoi(post)<len)
                    (*response)[atoi(post)] = '\0';
            }
        }
        if(lpbuf) 
            free(lpbuf);
    }while(0);

    return ret;
}

int login(tcpclient *pclient,char *page, char *username, char*passwd,char **response)
{
    //define
    int ret = True;
    char content_len[32] = "";
    char post[1024] = "";
    char host[100] = "";
    char header[1024] = "";
    char body[1024] = "";
    char *lpbuf = NULL;
    int len=0;

    //init
    memset(content_len,0, sizeof(content_len));
    memset(post,0, sizeof(post));
    memset(host,0, sizeof(host));
    memset(header,0, sizeof(header));
    memset(body,0, sizeof(body));    
    
    do{
        // 0. set body
        http_login_set_body(body, username, passwd);

        // 1. set method
        http_set_method(post,"POST", page);
        printf("%s\n",post);

        // 2. set host
        http_set_host(host , pclient);
        printf("%s\n",host);

        // 3. set headers  
        //http_set_header(header, "Connection", "Keep-Alive");
        http_set_header(header, "Content-Type", "multipart/form-data; boundary=----123");
        //http_set_header(header, "Cache-Control", "no-cache");
        printf("%s\n",header);

        // 4.set content_len
        http_set_content_len(content_len, body);
        printf("%s\n",content_len);

        len = strlen(post)+strlen(host)+strlen(header)+strlen(content_len)+strlen(body);

        // char *send_str = {
        //     "POST http://192.168.6.96:8000/api2/auth-token/ HTTP/1.1\r\n"
        //     "Host: 192.168.6.96:8000\r\n"
        //     "Content-Type:multipart/form-data; boundary=----123\r\n"
        //     "Content-Length: 166\r\n\r\n"
        //     "------123\r\n"
        //     "Content-Disposition: form-data; name=\"username\"\r\n\r\n"
        //     "homecloud@saming.com\r\n"
        //     "------123\r\n"
        //     "Content-Disposition: form-data; name=\"password\"\r\n\r\n"
        //     "123456\r\n"
        //     "------123--\r\n"};

        // char *ttt = {
        //     "------123\r\n"
        //     "Content-Disposition: form-data; name=\"username\"\r\n\r\n"
        //     "homecloud@saming.com\r\n"
        //     "------123\r\n"
        //     "Content-Disposition: form-data; name=\"password\"\r\n\r\n"
        //     "123456\r\n"
        //     "------123--\r\n"};
        // len = strlen(send_str) ;
        
        lpbuf = (char*)malloc(len);
        memset(lpbuf, '\0', len);
        if(lpbuf==NULL){
            ret = False;
            break;
        }
        
        strcpy(lpbuf,post);
        strcat(lpbuf,host);
        strcat(lpbuf,header);
        strcat(lpbuf,content_len);
        strcat(lpbuf,body);

        if(!connected_server(pclient , lpbuf, response, len))
            cout << "Error!" << endl;
    }while(0);

    return ret;
}
//

char * js_get_body(char * response)
{
    if(response == NULL)
    {
        return NULL;
    }
    char *buf = strstr(response, "{");
    
    char *buff = _find_string(buf,"}");
    return buff;
}

char * js_get_value_for_key(char*response, char* key)
{
    if(response == NULL || key == NULL)
    {
        return NULL;
    }
    //init
    char *ret = NULL;
    char *token = "token";
    //program
    char *buff = js_get_body(response);
    //cout << "body is :" << buff << endl;

    char *buf = strstr(response, token);
    buf = buf + strlen(token) + 3;
    ret = _find_string(buf,"\"");
    cout << "token is :" << ret << endl;

    return ret;
}

char *AnalyzeJS::get_value_for_key(char*buff, char* key, char* value)
{
    return NULL;
}


int forward(tcpclient *pclient,char *page, char* token,char **response)
{
    //define
    int ret = True;
    char post[1024] = "";
    char host[100] = "";
    char header[1024] = "";
    char end[] = "\r\n";
    char *lpbuf = NULL;
    int len=0;

    //init
    // end[0]='\r';
    // end[1]='\n';
    //memset(end,0, sizeof(end));
    memset(post,0, sizeof(post));
    memset(host,0, sizeof(host));
    memset(header,0, sizeof(header)); 
    
    do{
        //tcpclient_create(pclient,"127.0.0.1",8000);
        // char*username = "homecloud@saming.com";
        // char*passwd = "123456";
        // if(!login(pclient , GETTOKEN, username, passwd, response))   //获取token
        // {
        //     cout << "Error!" << __FUNCTION__ << __LINE__<< endl;
        //     break;
        // }

        // char *token = js_get_value_for_key(*response, "token");
        // if(!token)
        // {
        //     cout << "Error!" << __FUNCTION__ << __LINE__<< endl;
        //     break;
        // }
        // tcpclient_create(pclient,IP,8000);

        string Authorization =  " Token ";
        Authorization += token;

        // 1. set method
        http_set_method(post, "GET", page);
        printf("%s\n",post);

        // 2. set host
        http_set_host(host , pclient);
        printf("%s\n",host);

        // 3. set headers  
        http_set_header(header, "Authorization", (char *)Authorization.data());
        printf("%s\n",header);

        len = strlen(post)+strlen(host)+strlen(header)+sizeof(end) + 1;
        lpbuf = (char*)malloc(len);
        memset(lpbuf, '\0', len);
        if(lpbuf==NULL)
        {
            cout << "Error!" << __FUNCTION__ << "   " << __LINE__ << endl;
            ret = False;
            break;
        }

        strcpy(lpbuf,post);
        strcat(lpbuf,host);
        strcat(lpbuf,header);
        strcat(lpbuf,end);

        if(!connected_server(pclient , lpbuf, response, len))
        {
            cout << "Error!" << __FUNCTION__ << "   " << __LINE__ << endl;
            ret = False;
            break;
        }
    }while(0);

    return ret;

}


int getDataLibId(tcpclient *pclient,char *page, char *username, char*passwd,char **response)
{
    //define
    int ret = True;
    char content_len[32] = "";
    char post[1024] = "";
    char host[100] = "";
    char header[1024] = "";
    char body[1024] = "";
    char end[2] = "";
    char *lpbuf = NULL;
    int len=0;

    //init
    end[0]='\r';
    end[1]='\n';
    //memset(end,0, sizeof(end));
    memset(content_len,0, sizeof(content_len));
    memset(post,0, sizeof(post));
    memset(host,0, sizeof(host));
    memset(header,0, sizeof(header));
    memset(body,0, sizeof(body));    
    
    do{
        tcpclient_create(pclient,SELF_IP,8000);

        if(!login(pclient , GETTOKEN, username, passwd, response))   //获取token
        {
            cout << "Error!" << __FUNCTION__ << __LINE__<< endl;
            break;
        }

        char *token = js_get_value_for_key(*response, "token");
        if(!token)
        {
            cout << "Error!" << __FUNCTION__ << __LINE__<< endl;
            break;
        }
        string Authorization =  " Token ";
        Authorization += token;

        tcpclient_create(pclient,SELF_IP,8000);
        // 1. set method
        http_set_method(post, "GET", page);
        printf("%s\n",post);

        // 2. set host
        http_set_host(host , pclient);
        printf("%s\n",host);

        // 3. set headers  
        http_set_header(header, "Authorization", (char *)Authorization.data());
        printf("%s\n",header);

        len = strlen(post)+strlen(host)+strlen(header)+sizeof(end) + 1;
        lpbuf = (char*)malloc(len);
        memset(lpbuf, '\0', len);
        if(lpbuf==NULL){
            ret = False;
            break;
        }
        
        strcpy(lpbuf,post);
        strcat(lpbuf,host);
        strcat(lpbuf,header);
        strcat(lpbuf,end);

        if(!connected_server(pclient , lpbuf, response, len))
            cout << "Error!" << __FUNCTION__ << "   " << __LINE__ << endl;
    }while(0);

    return ret;
}



int write_file(char *buffer)
{
    int fd = open("/home/pi/liu.txt", O_RDWR|O_CREAT);
    write(fd, (void*)buffer, strlen(buffer));
    return 1;
}




int sameAccountLogin(tcpclient *pclient,char *page,char *my_method, char *username, char*passwd, char *mac,char **response)
{
    //define
    int ret = True;
    char content_len[32] = "";
    char post[1024] = "";
    char host[100] = "";
    char header[1024] = "";
    char body[1024] = "";
    char *lpbuf = NULL;
    char *ptmp = NULL;
    int len=0;

    //init
    memset(content_len,0, sizeof(content_len));
    memset(post,0, sizeof(post));
    memset(host,0, sizeof(host));
    memset(header,0, sizeof(header));
    memset(body,0, sizeof(body));    
    
    do{
        // 0. set body
        http_same_account_set_body(body,my_method, username, passwd, mac);

        // 1. set method
        http_set_method(post,method[POST], page);
        printf("%s\n",post);

        // 2. set host
        http_set_host(host , pclient);
        printf("%s\n",host);

        // 3. set headers  
        http_set_header(header, "Connection", "Keep-Alive");
        http_set_header(header, "Content-Type", "multipart/form-data; boundary=----123");
        printf("%s\n",header);

        // 4.set content_len
        //sprintf(content_len,"%d\r\n\r\n",strlen(body)+1);
        http_set_content_len(content_len, body);

        len = strlen(post)+strlen(host)+strlen(header)+strlen(content_len)+strlen(body)+1;
        lpbuf = (char*)malloc(len);
        if(lpbuf==NULL){
            ret = False;
            break;
        }
        strcpy(lpbuf,post);
        strcat(lpbuf,host);
        strcat(lpbuf,header);
        strcat(lpbuf,content_len);
        strcat(lpbuf,body);

        //write_file(lpbuf);

        if(!pclient->connected){
            tcpclient_conn(pclient);
        }

        if(tcpclient_send(pclient,lpbuf,len)<0){
            printf("tcpclient_send Error!");
            ret = False;
            break;
        }
        printf("发送请求:\n%s\n",lpbuf);

        /*释放内存*/
        if(lpbuf != NULL) free(lpbuf);
        lpbuf = NULL;

        /*it's time to recv from server*/
        if(tcpclient_recv(pclient,&lpbuf,0) <= 0){
            if(lpbuf) free(lpbuf);
            ret = False;
            break;
        }
        printf("接收响应:\n%s\n",lpbuf);

        /*响应代码,|HTTP/1.0 200 OK|
        *从第10个字符开始,第3位
        * */
        memset(post,0,sizeof(post));
        strncpy(post,lpbuf+9,3);
        if(atoi(post)!=200){
            if(lpbuf) free(lpbuf);
            //return atoi(post);
            ret = False;
            break;
        }

        ptmp = (char*)strstr(lpbuf,"\r\n\r\n");
        if(ptmp == NULL){
            free(lpbuf);
            ret = False;
            break;
        }
        ptmp += 4;/*跳过\r\n*/

        len = strlen(ptmp)+1;
        *response=(char*)malloc(len);
        if(*response == NULL){
            if(lpbuf) free(lpbuf);
            ret = False;
            break;
        }
        memset(*response,0,len);
        memcpy(*response,ptmp,len-1);

        /*从头域找到内容长度,如果没有找到则不处理*/
        ptmp = (char*)strstr(lpbuf,"Content-Length:");
        if(ptmp != NULL){
            char *ptmp2;
            ptmp += 15;
            ptmp2 = (char*)strstr(ptmp,"\r\n");
            if(ptmp2 != NULL){
                memset(post,0,sizeof(post));
                strncpy(post,ptmp,ptmp2-ptmp);
                if(atoi(post)<len)
                    (*response)[atoi(post)] = '\0';
            }
        }
        if(lpbuf) 
            free(lpbuf);

    }while(0);

    return ret;
}

int http_post(tcpclient *pclient,char *page,char *request,char **response)
{

    char post[1024],host[100],content_len[100];
    char *lpbuf,*ptmp;
    int len=0;

    lpbuf = NULL;
    //const char *header2="User-Agent: Tuobao Http 1.1\r\nCache-Control: no-cache\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept: */*\r\n";
    const char *header2="Connection: Keep-Alive\r\nContent-Type: multipart/form-data; boundary=----123\r\n";

    sprintf(post,"POST %s HTTP/1.1\r\n",page);
    sprintf(host,"Host: %s:%d\r\n",pclient->remote_ip,pclient->remote_port);
    sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(request)+1);

    len = strlen(post)+strlen(host)+strlen(header2)+strlen(content_len)+strlen(request)+1;
    lpbuf = (char*)malloc(len);
    if(lpbuf==NULL){
        return -1;
    }

    strcpy(lpbuf,post);
    strcat(lpbuf,host);
    strcat(lpbuf,header2);
    strcat(lpbuf,content_len);
    strcat(lpbuf,request);


    if(!pclient->connected){
        tcpclient_conn(pclient);
    }

    if(tcpclient_send(pclient,lpbuf,len)<0){
        printf("tcpclient_send Error!");
        return -1;
    }
    printf("发送请求:\n%s\n",lpbuf);

    /*释放内存*/
    if(lpbuf != NULL) free(lpbuf);
    lpbuf = NULL;

    //close(pclient->socket);

    /*it's time to recv from server*/
    if(tcpclient_recv(pclient,&lpbuf,0) <= 0){
        if(lpbuf) free(lpbuf);
        return -2;
    }
    printf("接收响应:\n%s\n",lpbuf);

    /*响应代码,|HTTP/1.0 200 OK|
     *从第10个字符开始,第3位
     * */
    memset(post,0,sizeof(post));
    strncpy(post,lpbuf+9,3);
    if(atoi(post)!=200){
        if(lpbuf) free(lpbuf);
        return atoi(post);
    }


    ptmp = (char*)strstr(lpbuf,"\r\n\r\n");
    if(ptmp == NULL){
        free(lpbuf);
        return -3;
    }
    ptmp += 4;/*跳过\r\n*/

    len = strlen(ptmp)+1;
    *response=(char*)malloc(len);
    if(*response == NULL){
        if(lpbuf) free(lpbuf);
        return -1;
    }
    memset(*response,0,len);
    memcpy(*response,ptmp,len-1);

    /*从头域找到内容长度,如果没有找到则不处理*/
    ptmp = (char*)strstr(lpbuf,"Content-Length:");
    if(ptmp != NULL){
        char *ptmp2;
        ptmp += 15;
        ptmp2 = (char*)strstr(ptmp,"\r\n");
        if(ptmp2 != NULL){
            memset(post,0,sizeof(post));
            strncpy(post,ptmp,ptmp2-ptmp);
            if(atoi(post)<len)
                (*response)[atoi(post)] = '\0';
        }
    }

    if(lpbuf) free(lpbuf);

    return 0;
}

// int main()
// {

//     //define 
//     tcpclient client[100];
//     //char *url = "http://192.168.6.96:8000/saming/singleLogin/";
//     char *url = "http://192.168.6.96:8000/api2/auth-token/";
//     char *my_method = "login";
//     char *username = "homecloud@saming.com";
//     char *passwd = "123456";
//     char *mac = "BE:ED:U8:OO:LI";

//     char *response = NULL;

//     //init
//     memset(client,0, sizeof(client));
    
//     //program
//     for(int i=0; i<100; i++)
//     {
//         do{
//             printf("开始组包\n");
//             tcpclient_create(&client[i],"192.168.6.96",8000);
//             if(!login(&client[i],url, username, passwd,&response))
//             {
//                 printf("失败!\n");
//                 break;
//             }
//             printf("响应:\n%d:%s\n",strlen(response),response);
//             free(response);   
//         }while(0);

//         close(client[i].socket);
//     }

//     return 0;
// }