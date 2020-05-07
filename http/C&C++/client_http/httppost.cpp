
#include "tcpclient.h"

char * test_login(char* url, char *username, char*passwd,char * my_method, char * mac, char * token)
{
    //if
    if(url== NULL){
        cout << "Error!" << __FUNCTION__ << "  " << __LINE__ << endl;
        return NULL;
    }

    //define 
    tcpclient client;
    char *response = NULL;
    int if_only_url = 1;

    //只有URl的话，直接转发url访问seafile.
    if(username==NULL || passwd==NULL || my_method==NULL || mac==NULL)
    {
        if(token)
        {
            if_only_url = 1;
        }
        else
        {
            //无 token
            cout << "Error!" << __FUNCTION__ << "  " << __LINE__ << endl;
            return NULL;
        }
    }
    cout << "if_only_url = [" << if_only_url << "]" << endl;

    //init
    memset(&client,0, sizeof(client));
    
    do{
        if(!tcpclient_create(&client,SELF_IP,8000))
        {
            cout << "Error!" << __FUNCTION__ << "  " << __LINE__ << endl;
            break;
        }
        //只有url，不用组http包，直接转发。
        if(if_only_url)
        {
            if(!forward(&client, url , token, &response))
            {
                cout << "Error!" << __FUNCTION__ << "  " << __LINE__ << endl;
                break;
            }
        }
        //有其他参数时， 需要组http包后，转发。
        else            
        {
            if(!login(&client,url, username, passwd,&response))
            //if(!sameAccountLogin(&client,url, my_method, username, passwd, mac,&response))
            {
                cout << "sameAccountLogin Error!" << __FUNCTION__ << "  " << __LINE__ << endl;
                close(client.socket);
                break;
            }
        }
        
        close(client.socket);
    }while(0);

    return response;
}

#define test
int main()
{

    //define 
    tcpclient client;
    //char *url = "http://192.168.6.96:8000/saming/singleLogin/"; //同账号登陆。
    char *url_getoken = "http://192.168.6.96:8000/api2/auth-token/";       //获取token
    char *url = "http://192.168.6.96:8000/api2/repos/";
    char *my_method = "login";
    char *username = "homecloud@saming.com";
    char *passwd = "123456";
    char *mac = "BE:ED:U8:OO:LI";
    char * token   = "beab2f460d0bfbd9df80120d1c168bb33de5544e";

    char *response = NULL;

    //init
    memset(&client,0, sizeof(client));
    
    //program
    do{
        printf("开始组包\n");
#ifdef login
        tcpclient_create(&client,"192.168.6.96",8000);
        if(!login(&client , url_getoken, username, passwd,&response))   //获取token
        {
            cout << "Error!" << __FUNCTION__ << __LINE__<< endl;
            break;
        }
#endif


#ifdef datalib
        if (!getDataLibId(&client, url, username, passwd, &response))
        {
            cout << "Error!" << __FUNCTION__ << __LINE__<< endl;
            break;
        }
#endif

#ifdef test
        char * response = test_login(url, username,passwd, my_method ,mac,token);
#endif

        if(response) free(response);   
    }while(0);

    close(client.socket);

    return 0;
}