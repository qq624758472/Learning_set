
#include "server.h"
#include<python2.7/Python.h>
#include <stdio.h>
#include <unistd.h>

#define DEFAULT_PORT 8080

void cb_stop_process(struct evhttp_request *req, void *arg);
void http_request_cb(struct evhttp_request *req, void *arg);
void print_request(struct evhttp_request *req, void *arg);
void php_request_cb(struct evhttp_request *req, void *arg);
void http_status_cb(struct evhttp_request *req, void *arg);
void get_history_operating_datalib(struct evhttp_request *req, void *arg);
int if_continue_operating();
int rm_sign();
int add_sign();
char * get_time_dir();
int if_exit_operating_process();
int if_process_exit(char name[]);



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
    
    evhttp_bind_socket(http, "0.0.0.0", port);
    
    event_base_dispatch(base);
    
    return 0;
}


int write_mount_posion_conf_file(char * mount_posion)
{
    FILE * fp = NULL;

    fp = fopen("/home/pi/work/conf/mount_posion.conf", "w+");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return -1;
    }
    if (fputs(mount_posion,fp) == NULL)
    {
        printf("fputs error\n");
        return -1;
    }
    fclose(fp);

    return 1;
}


int write_conf_file(char * info,char * file_path)
{
    FILE * fp = NULL;

    fp = fopen(file_path, "w+");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return -1;
    }
    if (fputs(info,fp) == NULL)
    {
        printf("fputs error\n");
        return -1;
    }
    fclose(fp);

    return 1;
}

int write_share_or_person_data_conf_file(char * share_or_person)
{
    FILE * fp = NULL;

    fp = fopen("/home/pi/work/conf/share_or_person.conf", "w+");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return -1;
    }
    if (fputs(share_or_person,fp) == NULL)
    {
        printf("fputs error\n");
        return -1;
    }
    fclose(fp);

    return 1;
}


int write_username_conf_file(char * username)
{
    FILE * fp = NULL;

    fp = fopen("/home/pi/work/conf/username.conf", "w+");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return -1;
    }
    if (fputs(username,fp) == NULL)
    {
        printf("fputs error\n");
        return -1;
    }
    fclose(fp);

    return 1;
}

int write_passwd_conf_file(char * passwd)
{
    FILE * fp = NULL;

    fp = fopen("/home/pi/work/conf/passwd.conf", "w+");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return -1;
    }
    if (fputs(passwd,fp) == NULL)
    {
        printf("fputs error\n");
        return -1;
    }
    fclose(fp);

    return 1;
}

int get_conf(buffer,filePath)
{
    FILE * fp = NULL;
    fp = fopen(filePath, "r");
    if(fp == NULL)
    {
        printf("fopen error\n");
        return -1;
    }
    if (fgets(buffer,10240,fp) == NULL)
    {
        printf("fgets error\n");
        return -1;
    }
    fclose(fp);
    return 1;
}

int * thread_export(void *arg)
{
    /* 子进程程序 */
    system("sudo python /home/pi/work/export/export.py");
    rm_sign();//解锁
    return NULL;
}


int * thread_import(void *arg)
{
    /* 子进程程序 */
    system("sudo python /home/pi/work/import/import.py");
    rm_sign();//解锁
    return NULL;
}

void php_request_cb(struct evhttp_request *req, void *arg)    //给服务器发送命令。 
{
    printf("run php_request_cb...\n");
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    int command_int = 0;
    char buffer[10240] = {0};
    char *return_success = "start";

    if_exit_operating_process();

    if (if_continue_operating() == -1)
    {
        char * error = ERR101;  
        memcpy(buffer,error,strlen(error));
        goto access_response;  //被要求返回200状态码。
    }

    //获取发送来的信息。
    evkey_input = evhttp_request_get_input_headers(req);
    const char *command = evhttp_find_header(evkey_input,"command"); //导出功能： 挂载fuse， 并拷贝指定用户下的文件到插入的磁盘中。
    const char *username = evhttp_find_header(evkey_input,"username");
    const char *mount_posion = evhttp_find_header(evkey_input,"mount_posion");   //需要导出或导入的挂载的位置
    const char *passwd = evhttp_find_header(evkey_input,"passwd");
    const char *visits_num = evhttp_find_header(evkey_input,"visits");

    system("echo Start write conf! > /home/pi/work/conf/write_conf.log");
    if((strcmp(command,"10") == 0) || (strcmp(command,"11") == 0))//导入需要加个字段，share_or_person:1导入到私人资料库     2导入到共享。   3.共享+私密
    {
        const char *share_or_person = evhttp_find_header(evkey_input,"share_or_person");
        if(share_or_person == NULL)
        {
            printf("share_or_person is NULL\n");
            goto error_response;
        }
        //获取到挂载的位置要存到配置文件中
        if (write_share_or_person_data_conf_file(share_or_person) == -1)
        {
            system("echo Get share_or_person ERROR >> /home/pi/work/conf/write_conf.log");
            goto error_response;
        }
    }

    if(strcmp(command,"13") == 0)//恢复 功能需要加字段。
    {
        if(visits_num == NULL)
        {
            printf("visits_num is NULL\n");
            char return_tmp[] = "visits_num is NULL！";
            memcpy(buffer,return_tmp,sizeof(return_tmp));
            goto error_response;
        }
    }

    if(command == NULL || username == NULL || mount_posion == NULL || passwd == NULL)
    {
        printf("command or username is NULL\n");
        goto error_response;
    }
    else 
    {
        //获取到挂载的位置要存到配置文件中
        if (write_mount_posion_conf_file(mount_posion) == -1)
        {
            system("echo Get mount_posion ERROR >> /home/pi/work/conf/write_conf.log");
            goto error_response;
        }
        //获取到用户名要存到配置文件中
        if (write_username_conf_file(username) == -1)
        {
            system("echo Get username ERROR >> /home/pi/work/conf/write_conf.log");
            goto error_response;
        }
        //获取到密码要存到配置文件中
        if (write_passwd_conf_file(passwd) == -1)
        {
            system("echo Get passwd ERROR >> /home/pi/work/conf/write_conf.log");
            goto error_response;
        }

        //int command_int = atoi(command);  //树莓派里用不了atoi   直接段错误。
        if (strcmp(command , "10") == 0)
        {
            command_int = CMD_IMPORT;
        }
        else if(strcmp(command , "11") == 0)
        {
            command_int = CMD_EXPORT;
        }
        else if(strcmp(command , "12") == 0)
        {
            command_int = CMD_BACKUP;
        }
        else if(strcmp(command , "13") == 0)
        {
            command_int = CMD_RESTORE;
        }
        else
        {
            printf("command not found\n");
            goto error_response;
        }

        printf("command: [%s].    command_int: [%d]\n", command,command_int);
        switch(command_int)
        {
            case CMD_IMPORT:   //导入
            {
                add_sign();//加锁

                pthread_t id;
                if(pthread_create(&id,NULL,(void *)thread_import,NULL))
                {
                    system("echo pthread_create Error!! > /home/pi/work/data_control_server.log");
                    goto error_response;
                }
                pthread_detach (id);//非阻塞回收线程结束后的资源。
                /* 主线程程序*/
                memcpy(buffer,return_success,strlen(return_success));
                goto access_response;
            }
            break;

            case CMD_EXPORT:    //导出
            {
                //导出分两步操作，1.判断是否插入硬盘，并且存在容量大于等于原盘分区的分区。 
                //             2.使用seaf-fuse.sh 恢复数据。      
                //             3.拷贝指定用户数据到插入的磁盘中。
                //system("sudo /home/pi/work/seafile-bak.sh");
                add_sign();//加锁        

                pthread_t id;
                if(pthread_create(&id,NULL,(void *)thread_export,NULL))
                {
                    system("echo pthread_create Error!! > /home/pi/work/data_control_server.log");
                    goto error_response;
                }
                pthread_detach (id);//非阻塞回收线程结束后的资源。
                /* 父进程程序*/
                memcpy(buffer,return_success,strlen(return_success));
                goto access_response;
            }
            break;

            case CMD_BACKUP:    //备份
            {
                add_sign();//加锁
                if ( fork() == 0 ) 
                {   
                    /* 子进程程序 */
                    system("sudo python /home/pi/work/backup/backup.py");
                    rm_sign();//解锁
                    return;
                }   
                else 
                {   
                    /* 父进程程序*/
                    memcpy(buffer,return_success,strlen(return_success));
                    goto access_response;
                }
            }
            break;

            case CMD_RESTORE:   //恢复
            {
                add_sign();//加锁
                //visits_num  用来判断用户的第几次访问。 第一次：获取存在备份文件夹的路径。
                if(strcmp(visits_num,"1") == 0)
                {
                    //会生成存在备份文件夹的路径。
                    system("sudo python /home/pi/work/revert/listdir.py");
                    //写入路径到配置文件。
                    char filePath[] = "/home/pi/work/conf/RestorePoint.conf";
                    if(get_conf(buffer,filePath) == -1)
                    {
                        char return_tmp[] = "Get [/home/pi/work/conf/RestorePoint.conf] Error!";
                        memcpy(buffer,return_tmp,strlen(return_tmp));
                        rm_sign();//解锁
                        goto error_response;
                    }
                } 
                else  //第二次访问进行恢复操作。需要备份的文件夹名（就是一个以时间命名的文件夹）
                {
                    char *conf_back_path = "/home/pi/work/conf/revert_back_path.conf";
                    const char *back_path = evhttp_find_header(evkey_input,"back_path");
                    if(back_path == NULL)
                    {
                        char return_tmp[] = "back_path is NULL!";
                        memcpy(buffer,return_tmp,strlen(return_tmp));
                        rm_sign();//解锁
                        goto error_response;
                    }
                    //获取到还原点位置存到配置文件中
                    if (write_conf_file(back_path,conf_back_path) == -1)
                    {
                        system("echo write /home/pi/work/conf/revert_back_path.conf Error!>> /home/pi/work/conf/write_conf.log");
                        char return_tmp[] = "write /home/pi/work/conf/revert_back_path.conf Error!";
                        memcpy(buffer,return_tmp,strlen(return_tmp));
                        rm_sign();//解锁
                        goto error_response;
                    }
                    //写入配置文件后，调用Python文件来还原。
                    if ( fork() == 0 ) 
                    {   
                        /* 子进程程序 */
                        system("sudo python /home/pi/work/revert/revert.py");
                        rm_sign();//解锁                        
                        return;
                    }   
                    else 
                    {   
                        /* 父进程程序*/
                        memcpy(buffer,return_success,strlen(return_success));
                        goto access_response;
                    }

                }
                rm_sign();//解锁
                goto access_response;
            }
            break;

            default:
            {
                char return_tmp[] = "Command Error!";
                memcpy(buffer,return_tmp,strlen(return_tmp));
                goto error_response;
            }
            break;
        }
    }

access_response:
    system("sync");//响应之前，刷新数据到磁盘，防止用户直接拔U盘。
    //响应客户端。
    //evbuffer_add_printf(evb, "<h>Control Server Access!<h>");
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Control-Server","access");
    evhttp_send_reply(req, HTTP_OK, "OK", evb); 
    evbuffer_free(evb);
    return;
error_response:
    system("sync");
    //响应客户端。
    //evbuffer_add_printf(evb, "<h>Control Server Error!<h>");
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Control-Server","access");
    evhttp_send_reply(req, 500, "ERROR", evb); 
    evbuffer_free(evb);
    return;
}

//是否可以进行下一步操作。    判断标记文件存在时，禁止下一步操作，如果标记文件不存在，进行下一步操作。
int if_continue_operating()
{
    if((access("/home/pi/work/conf/ifcontinue.conf",F_OK))!=-1)   
    {   
        printf("ifcontinue.conf is exist\n");   
        return -1;
    }   
    else  
    {   
        printf("ifcontinue.conf not is exist\n"); 
        return 1;
    }   
}

int add_sign()
{
    system("touch /home/pi/work/conf/ifcontinue.conf");
    return 1;
}

int rm_sign()
{
    system("rm /home/pi/work/conf/ifcontinue.conf");
    return 1;
}

//获取备份的时间文件夹。
char * get_time_dir()
{
    Py_Initialize();
    if ( !Py_IsInitialized() ) 
    { 
        return -1; 
    } 
    PyRun_SimpleString("import os");
    PyRun_SimpleString("import datetime");
    PyRun_SimpleString("import threading");
    PyRun_SimpleString("import inspect");
    PyRun_SimpleString("import ctypes");
    PyRun_SimpleString("import json");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/pi/work/backup')");

    PyObject *pName,*pMoudle,*pDict,*pFunc,*pArg;
    pName = PyString_FromString("py_get_time_dir");
    pMoudle = PyImport_Import(pName);
    if (!pMoudle)
    {
        printf("get moudle handle error");
        return -1;
    }

    pDict = PyModule_GetDict(pMoudle); 
    if ( !pDict ) 
    { 
        printf("get moudledict handle error");
        return -1; 
    }
    pFunc = PyDict_GetItemString(pDict,"py_get_time_dir"); 
    if ( !pFunc || !PyCallable_Check(pFunc) ) 
    { 
        printf("can't find function [add]"); 
        return -1; 
    } 
    char * c;
    //pArg = Py_BuildValue("(s)", username);
    PyObject *result = PyEval_CallObject(pFunc, NULL);
    PyArg_Parse(result, "s", &c);
    printf("%s\n",c);
    Py_Finalize();
    return c;
}


void cb_stop_process(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    char buffer[10240] = "0";
    char cmd[1024] = "0";
    //获取发送来的信息。
    evkey_input = evhttp_request_get_input_headers(req);
    const char *stop_command = evhttp_find_header(evkey_input,"stop_command");

    if(stop_command == NULL)    {
        char * error = "stop command is NULL!";
        memcpy(buffer,error, strlen(error));
        goto error_response;
    }

    if(memcmp(stop_command,"10",2) == 0)    {
        char * import = "import.py";
        sprintf(cmd,"ps -ef | grep %s | grep -v grep | cut -c 9-15 | xargs kill -s 9",import);
        printf("%s\n",cmd);

        int status = system(cmd);
        if((-1 != status) && (0 != WIFEXITED(status)) && (0 == WEXITSTATUS(status)))
        {
            memcpy(buffer,"import", 7);
            goto access_response;
        }
        else
        {
            memcpy(buffer,"102", 4);
            goto error_response;
        } 
    }
    else if(memcmp(stop_command,"11",2) == 0)   {
        char * export = "export.py";
        sprintf(cmd,"ps -ef | grep %s | grep -v grep | cut -c 9-15 | xargs kill -s 9",export);
        printf("%s\n",cmd);

        int status = system(cmd);
        if((-1 != status) && (0 != WIFEXITED(status)) && (0 == WEXITSTATUS(status)))
        {
            memcpy(buffer,"export",7);
            goto access_response;
        }
        else
        {
            memcpy(buffer,"102", 4);
            goto error_response;
        } 
    }
    else if(memcmp(stop_command,"12",2) == 0)   {
        char * backup = "backup.py";
        char * dir_time = get_time_dir();//获取备份的文件夹名称，因为强制停止，导致数据损坏，需要删除该文件夹
        
        memset(cmd,0,1024);
        sprintf(cmd,"ps -ef | grep %s | grep -v grep | cut -c 9-15 | xargs kill -s 9",backup);
        printf("%s\n",cmd);

        int status = system(cmd);

        system("ps -ef | grep rdiff-backup | grep -v grep | cut -c 9-15 | xargs kill -s 9");
        if((-1 != status) && (0 != WIFEXITED(status)) && (0 == WEXITSTATUS(status)))
        {
            memset(cmd,0,1024);
            sprintf(cmd,"rm %s -rf",dir_time);
            system(cmd);
            system(cmd);

            memcpy(buffer,"backup", 7);
            goto access_response;
        }
        else
        {
            memcpy(buffer,"102", 4);
            goto error_response;
        }   
    }
    else if(memcmp(stop_command,"13",2) == 0)   {
        char * revert = "revert.py";
        sprintf(cmd,"ps -ef | grep %s | grep -v grep | cut -c 9-15 | xargs kill -s 9",revert);
        printf("%s\n",cmd);

        int status = system(cmd);
        if((-1 != status) && (0 != WIFEXITED(status)) && (0 == WEXITSTATUS(status)))
        {
            memcpy(buffer,"revert", 7);
            goto access_response;
        }
        else
        {
            memcpy(buffer,"102", 4);
            goto error_response;
        } 
    }   
    else
    {
        memcpy(buffer,"command Error", 14);
        goto error_response;
    }

access_response:
    //强制停止成功时，初始化数据库。
    system("/home/pi/work/init_mysql.sh");

    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, HTTP_OK, "OK", evb); 
    evbuffer_free(evb);
    return;
error_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","error");
    evhttp_send_reply(req, HTTP_INTERNAL, "STATERR", evb); 
    evbuffer_free(evb);
    return;
}


void http_status_cb(struct evhttp_request *req, void *arg)   //获取服务器状态。
{
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    FILE * fp = NULL;
    char buffer[10240] = "0";
    int again_mount  = 0;

    int command_int = 0;

    if_exit_operating_process();

    //判断是否加锁
    if (if_continue_operating() == -1)
    {
        char * error = ERR101;  
        memcpy(buffer,error,strlen(error));
        goto access_response;  //被要求返回200状态码。
    }

    add_sign();//加锁。

    //获取发送来的信息。
    evkey_input = evhttp_request_get_input_headers(req);
    char *username = evhttp_find_header(evkey_input,"username");
    char *passwd = evhttp_find_header(evkey_input,"passwd");

    system("sudo rm /home/pi/work/conf/write_conf.log");
    system("echo Start disk init write conf! >> /home/pi/work/conf/write_conf.log");
    if(passwd == NULL || username == NULL)
    {
        printf("passwd or username is NULL\n");
        rm_sign(); //解锁   不能放到goto里面，因为判断有锁后，也会跳入到Error里。 
        goto error_response;
    }
    //获取到用户名要存到配置文件中
    if (write_username_conf_file(username) == -1)
    {
        system("echo Get username ERROR >> /home/pi/work/conf/write_conf.log");
        rm_sign(); //解锁
        goto error_response;
    }
    //获取到密码要存到配置文件中
    if (write_passwd_conf_file(passwd) == -1)
    {
        system("echo Get passwd ERROR >> /home/pi/work/conf/write_conf.log");
        rm_sign(); //解锁
        goto error_response;
    }
    
    //删除所有残留日志文件。
    // system("sudo rm /home/pi/work/disk-init/disk-init.log");
    // system("sudo rm /home/pi/work/export/export.log");
    // system("sudo rm /home/pi/work/import/import.log");
    
    do {
        again_mount++;
        system("sudo /home/pi/work/disk-init/umount-all.sh");
        system("sudo python /home/pi/work/disk-init/disk-init.py");

        fp = fopen("/home/pi/work/disk-init/diskinfo.conf", "r");
        if(fp == NULL)
        {
            printf("fopen error\n");
            rm_sign(); //解锁
            goto error_response;
        }
        if (fgets(buffer,10240,fp) == NULL)
        {
            printf("fgets error\n");
            rm_sign(); //解锁
            goto error_response;
        }

        if (strncmp(buffer, ERR501 , 3 ) == 0) {
            printf("mount Error again!\n");
            fclose(fp);
            sleep(5);
            if(again_mount > 60) {
                printf("mount Error  no again!\n");
                rm_sign(); //解锁
                goto  error_501_response;
            }
            continue;
        }

        fclose(fp);
        rm_sign(); //解锁
        goto access_response;

    }while(1);



access_response:
    system("sync");//响应之前，刷新数据到磁盘，防止用户直接拔U盘。
    //响应客户端。
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, HTTP_OK, "OK", evb); 
    evbuffer_free(evb);
    return;
error_response:
    system("sync");//响应之前，刷新数据到磁盘，防止用户直接拔U盘。
    //响应客户端。
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, 500, "STATERR", evb); 
    evbuffer_free(evb);
    return;
error_501_response:
    system("sync");//响应之前，刷新数据到磁盘，防止用户直接拔U盘。
    //响应客户端。
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, 501, "STATERR", evb); 
    evbuffer_free(evb);
    return;
}

//查询历史记录
char * run_nasl(char * username)
{
    Py_Initialize();
    if ( !Py_IsInitialized() ) 
    { 
        return -1; 
    } 
    PyRun_SimpleString("import os");
    PyRun_SimpleString("import datetime");
    PyRun_SimpleString("import threading");
    PyRun_SimpleString("import inspect");
    PyRun_SimpleString("import ctypes");
    PyRun_SimpleString("import json");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/pi/work/write_datalib')");

    PyObject *pName,*pMoudle,*pDict,*pFunc,*pArg;
    pName = PyString_FromString("datalib");
    pMoudle = PyImport_Import(pName);
    if (!pMoudle)
    {
        printf("get moudle handle error");
        return -1;
    }

    pDict = PyModule_GetDict(pMoudle); 
    if ( !pDict ) 
    { 
        printf("get moudledict handle error");
        return -1; 
    }
    pFunc = PyDict_GetItemString(pDict,"get_operating_database"); 
    if ( !pFunc || !PyCallable_Check(pFunc) ) 
    { 
        printf("can't find function [add]"); 
        return -1; 
    } 
    pArg = Py_BuildValue("(s)", username);
    PyObject *result = PyEval_CallObject(pFunc, pArg);
    char *pout = PyString_AsString(result);
    Py_Finalize();
    return pout;
}

//清空历史记录
int empty_history_database(char * username)
{
    Py_Initialize();
    if ( !Py_IsInitialized() ) 
    { 
        return -1; 
    } 
    PyRun_SimpleString("import os");
    PyRun_SimpleString("import datetime");
    PyRun_SimpleString("import threading");
    PyRun_SimpleString("import inspect");
    PyRun_SimpleString("import ctypes");
    PyRun_SimpleString("import json");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/pi/work/write_datalib')");

    PyObject *pName,*pMoudle,*pDict,*pFunc,*pArg;
    pName = PyString_FromString("empty_database");
    pMoudle = PyImport_Import(pName);
    if (!pMoudle)
    {
        printf("get moudle handle error");
        return -1;
    }

    pDict = PyModule_GetDict(pMoudle); 
    if ( !pDict ) 
    { 
        printf("get moudledict handle error");
        return -1; 
    }
    pFunc = PyDict_GetItemString(pDict,"empty_operating_history_datalib"); 
    if ( !pFunc || !PyCallable_Check(pFunc) ) 
    { 
        printf("can't find function [add]"); 
        return -1; 
    } 
    int c= 0;
    pArg = Py_BuildValue("(s)", username);
    PyObject *result = PyEval_CallObject(pFunc, pArg);
    PyArg_Parse(result, "i", &c);
    printf("%d\n",c);
    Py_Finalize();
    return c;
}


//获取数据库，历史记录的备份信息。
void get_history_operating_datalib(struct evhttp_request *req, void *arg)   //获取服务器状态。
{
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    FILE * fp = NULL;
    char buffer[10240] = "Success";

    //获取发送来的信息。
    evkey_input = evhttp_request_get_input_headers(req);
    char *if_empty_database = evhttp_find_header(evkey_input,"if_empty_database");
    char *username = evhttp_find_header(evkey_input,"username");
    //char *command = evhttp_find_header(evkey_input,"command");
    if(if_empty_database == NULL || username == NULL)   {
        printf("if_empty_database == NULL || username == NULL");
        char * error = "if_empty_database == NULL || username == NULL";
        memcpy(buffer,error, strlen(error));
        goto error_response;
    }

    //清空历史记录
    if(strcmp(if_empty_database,"1") == 0)  
    {
        if (empty_history_database(username) == -1) 
        {
            char * empty_error = "Empty history database Error!";
            memcpy(buffer,empty_error, strlen(empty_error));
            goto error_response;
        }
        goto access_response;
    } 
    else 
    {    //查询历史记录
        char * history_json = run_nasl(username);
        if(history_json == NULL)
        {
            char * get_error = "Get history database Error!";
            memcpy(buffer,get_error, strlen(get_error));
            goto error_response;
        }
        memcpy(buffer,history_json,strlen(history_json));
        goto access_response;
    }

    

access_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, HTTP_OK, "OK", evb); 
    evbuffer_free(evb);
    return;
error_response:
    evbuffer_add_printf(evb, buffer);
    evkey_output = evhttp_request_get_output_headers(req);
    evhttp_add_header(evkey_output,"Status-Server","access");
    evhttp_send_reply(req, 500, "STATERR", evb); 
    evbuffer_free(evb);
    return;
}


int * thread(void *arg)
{
    
    sleep(10);
    printf("ER Zi.\n");
    return NULL;
}

void http_request_cb(struct evhttp_request *req, void *arg) 
{
    struct evbuffer *evb = evbuffer_new();
    struct evkeyvalq *evkey_output = NULL;
    struct evkeyvalq *evkey_input = NULL;
    FILE * fp = NULL;
    char buffer[10240] = "<h1>test</h1>";



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

void print_request(struct evhttp_request *req, void *arg) 
{
    const char *cmdtype;
    struct evkeyvalq *headers;
    struct evkeyval *header;
    struct evbuffer *buf;
    
    switch (evhttp_request_get_command(req)) 
    {
        case EVHTTP_REQ_GET: cmdtype = "GET"; break;
        case EVHTTP_REQ_POST: cmdtype = "POST"; break;
        case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
        case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
        case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
        case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
        case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
        case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
        case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
        default: cmdtype = "unknown"; break;
    }
    
    printf("Received a %s request for %s\n",
           cmdtype, evhttp_request_get_uri(req));
    
    headers = evhttp_request_get_input_headers(req);
    puts("Headers:\n");
    for (header = headers->tqh_first; header; header = header->next.tqe_next) 
    {
        printf("  %s: %s\n", header->key, header->value);
    }
    
    buf = evhttp_request_get_input_buffer(req);
    puts("Input data: <<<");
    while (evbuffer_get_length(buf)) 
    {
        int n;
        char cbuf[128];
        n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
        if (n > 0)
        {
            (void) fwrite(cbuf, 1, n, stdout);
        }
    }
    puts(">>>");
}


//是否存在互拷功能的四个模块在进程中， 如果存在任意一个，则不进行操作， 如果都不存在则，删除conf/ifcontinue.conf文件。
int if_exit_operating_process()
{
    if(if_process_exit("import.py")==1) {
        goto return_1;
    }

    if(if_process_exit("export.py")==1) {
        goto return_1;
    }

    if(if_process_exit("backup.py") == 1) {
        goto return_1;
    }

    if(if_process_exit("revert.py") == 1) {
        goto return_1;
    }

    if(if_process_exit("listdir.py") == 1) {
        goto return_1;
    }

    if(if_process_exit("disk-init.py") == 1) {
        goto return_1;
    }

    char cmd[1024] = "0";
    sprintf(cmd,"rm %s",CONF_LOCK);
    int status = system(cmd);
    if((-1 != status) && (0 != WIFEXITED(status)) && (0 == WEXITSTATUS(status)))
    {
        goto return_none;
    }

return_none:
    return -1;
return_1:
    return 1;
}

//判断某个进程是否存在。存在：返回1     不存在：返回-1
int if_process_exit(char name[])
{
    FILE   *stream; 
    char   buf[1024]; 
    char cmd[1024] = "0";

    memset( buf, 0, sizeof(buf) );
    sprintf(cmd,"ps -x |grep %s | grep -v grep",name);
    printf("%s\n",cmd);
    stream = popen(cmd , "r" );
    fread( buf, sizeof(char), sizeof(buf), stream); //将刚刚FILE* stream的数据流读取到buf中
    //printf("if_process_exit is [%s]\n", buf );

    //wait(stream);  // 不回收资源会导致僵尸进程。 注释是因为加入这个判断不出来有互拷模块进程在运行。  不用等待接收，关闭就行了。python下调用会产生僵尸进程。
    pclose(stream);
    if(buf[0] == 0) {
        return -1;
    }else {
        return 1;
    }
}