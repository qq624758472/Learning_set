#include "stream.h"


int FileRead::operating()
{
    char buff[2048];
    memset(buff, '\0', sizeof(buff));

    //以读方式打开文件。
    ifstream fileread;
    fileread.open(PATH);

    //从文件中获取数据到buff中。
    fileread >> buff;

    cout << buff << endl;
    
    fileread.close();
	return 0;
}


int FileWrite::operating()
{
    char buff[2048];

    memset(buff, '\0', sizeof(buff));

    //写模式打开文件。
    ofstream filewrite;
    filewrite.open(PATH);

    //获取用户数据到buff中。
    cin.getline(buff, 100);

    //写入数据到文件。
    filewrite << buff << endl;  

    //关闭
    filewrite.close();
	return 0;
}