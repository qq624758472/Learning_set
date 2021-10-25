#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#define CONF_LOCK "/home/pi/work/conf/ifcontinue.conf"    //文件锁

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
    if(buf[0] == 0) {
        return -1;
    }else {
        return 1;
    }
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
	printf("%s\n",cmd);
    // int status = system(cmd);
    // if((-1 != status) && (0 != WIFEXITED(status)) && (0 == WEXITSTATUS(status)))
    // {
	// 	printf("rm success\n");
    //     goto return_none;
    // }
return_none:
    return -1;
return_1:
    return 1;
}

int main( void ) 
{ 
	if_exit_operating_process();
    return 0;
} 
