#include <stdio.h>
#include <stdlib.h>
#include <event2/keyvalq_struct.h>
#include <event2/http.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <unistd.h>
#include <fcntl.h>
#include "Error_List.h"
#include <pthread.h>
//#include <evhtp.h>








//命令
#define CMD_IMPORT  10      //导入
#define CMD_EXPORT  11      //导出
#define CMD_BACKUP  12      //备份
#define CMD_RESTORE 13      //还原


//配置文件
#define CONF_LOCK "/home/pi/work/conf/ifcontinue.conf"    //文件锁
