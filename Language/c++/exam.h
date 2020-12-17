#ifndef __EXAM__
#define __EXAM__

#include <iostream>
#include <unistd.h>
#include <vector>
#include <string.h>
using namespace std;
/**
 * 
 * -------------------------------------------------------------------------------------------------------------------
 * 实现一个自定义小工具
 * -------------------------------------------------------------------------------------------------------------------
 * 功能要求
 * - desktop文件格式无错误时进程退出码为0
 * - 实现对desktop文件Name、Exec、Icon三个字段的解析
 * - 需要检测文件的格式，遇到错误时以退出码 1 退出进程
 * - 输入接收一个文件的绝对路径，能够输出程序名称、图标文件
 * 
 * 示例
 * - aaa.desktop文件内容为:
 *     [Desktop Entry]
 *     Name=123;
 *     Name[zh_CN]="我是123";
 *     Exec=ls %f
 *     Icon=/tmp/b.icon
 * 
 * - 输入参数（-d指定desktop文件的绝对路径，-n表示要获取应用程序名称）：
 *     a.out -d /home/a/a.desktop -n
 * - 输出结果：
 *     A或我是A（中文环境下，注意应该去掉引号）
 * 
 * - 输入参数（-i表示要获取应用程序的图标）
 *     a.out -d /home/a/a.desktop -i
 * - 输出结果
 *     /tmp/a.png
 * 
 * - 输入参数（-e表示要启动此应用程序，其后可根一个或多个传入参数）
 *     a.out -d /home/a/a.desktop -e /home/a/test.txt
 * - 输出结果：
 *     输出结果为 /home/a/test.txt 文件的内容，也就是Exec命令执行时对标准输出通道所写入的全部数据
 * 
 */





#define DEBUG_EXAM

#define DESKTOP_HEAD "[Desktop Entry]"
#define NAME "Name"
#define EXEC "Exec"
#define ICON "Icon"


void repalce_all_ditinct(string& str,const string& old_value,const string& new_value);

//文件操作类, 文件的所有操作都在这.
class FileOperation
{
private:
//define
    string path;
public:
    vector<string> fileBuf; //文件内容,按行读取放入容器中.
    vector<string> name;  //程序名称集合
    vector<string> exec;  //程序可执行文件集合
    vector<string> icon;  //程序图标文件集合

public:
//init
    FileOperation(string path);
    ~FileOperation();


//pro
    //更新path路径
    void updatePath(string path);

    //判断文件是否存在
    bool isExist();

    //判断文件格式是否正确
    bool isFormat();

    //判断path是否初始化
    bool isPathInit();


private:
    //从该行取出值到对应vector中
    bool getStringToBuf(string buf);
};




#endif