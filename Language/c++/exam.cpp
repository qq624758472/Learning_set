#include <iostream>
#include "exam.h"
#include <fstream> //文件流操作的头文件
using namespace std;

FileOperation::FileOperation(string patch)
{
    this->path = path;
}

FileOperation::~FileOperation()
{
}

//判断文件是否存在
bool FileOperation::isExist()
{
    if (access(path.c_str(), F_OK) == 0)
        return true;
    else
        return false;
}

//判断文件格式是否正确
bool FileOperation::isFormat()
{
    vector<string>::iterator head;
    int num = 0; //统计符合要求的行数.

    fstream fin(path.c_str()); //打开文件
    string readline;
    while (getline(fin, readline)) //逐行读取，直到结束
    {
        fileBuf.push_back(readline);
    }

    //文件内容为空,返回报错
    if (fileBuf.empty())
    {
        cout << "file is empty" << endl;
        return false;
    }

#ifdef DEBUG_EXAM
    //读出来的文件内容显示
    {
        vector<string>::iterator v = fileBuf.begin();
        while (v != fileBuf.end())
        {
            cout << "行内容为:" << *v << endl;
            v++;
        }
    }
#endif

    //首行文件内容判断.
    {
        vector<string>::iterator v = fileBuf.begin();
        while (*v == "") //屏蔽首行前都为空行的情况.
        {
            v++;
        }
        head = v; //记录首行位置.
        if (*v != DESKTOP_HEAD)
        {
            cout << "head is not " << DESKTOP_HEAD << endl;
            goto error; //首行格式不对,返回报错.
        }
    }
    //判断是否有Name, Exec, Icon字段
    {
        vector<string>::iterator v = head;
        v++;
        while (v != fileBuf.end())
        {
            string tmp = *v;
            if ((!tmp.compare(0, 3, NAME, 0, 3)) || (!tmp.compare(0, 3, EXEC, 0, 3)) || (!tmp.compare(0, 3, ICON, 0, 3)))
            {
                getStringToBuf(tmp); //说明该行格式符合要求,保存到vector中.
                num++;
            }
            v++;
        }
        if (!num)
        {
            cout << "不存在需要的字段" << endl;
            goto error;
        }
    }

#ifdef DEBUG_EXAM //调试代码
    {
        vector<string>::iterator v = name.begin();
        while (v != name.end())
        {
            cout << "name:" << *v << endl;
            v++;
        }
    }
    {
        vector<string>::iterator v = exec.begin();
        while (v != exec.end())
        {
            cout << "exec:" << *v << endl;
            v++;
        }
    }
    {
        vector<string>::iterator v = icon.begin();
        while (v != icon.end())
        {
            cout << "icon:" << *v << endl;
            v++;
        }
    }
#endif
access:
    return true;
error:
    return false;
}

//更新path路径
void FileOperation::updatePath(string path)
{
    this->path = path;
}

//从该行取出值到对应vector中
bool FileOperation::getStringToBuf(string buf)
{
    if (buf.empty())
    {
        cout << "buf is empty" << endl;
        goto error;
    }

    if (!buf.compare(0, 3, NAME, 0, 3))
    {
        for (int i = 0; i < buf.length(); i++)
        {
            if (buf[i] == '=')
            {
                name.push_back(buf.substr(++i));
                goto access;
            }
        }
    }
    else if (!buf.compare(0, 3, EXEC, 0, 3))
    {
        for (int i = 0; i < buf.length(); i++)
        {
            if (buf[i] == '=')
            {
                exec.push_back(buf.substr(++i));
                goto access;
            }
        }
    }
    else if (!buf.compare(0, 3, ICON, 0, 3))
    {
        for (int i = 0; i < buf.length(); i++)
        {
            if (buf[i] == '=')
            {
                icon.push_back(buf.substr(++i));
                goto access;
            }
        }
    }
    else
    {
        cout << "ERROR" << endl;
    }

error:
    return false;
access:
    return true;
}

//判断path是否初始化
bool FileOperation::isPathInit()
{
    if (path.empty())
        return false;
    else
        return true;
}


//将一个字符串中的特定字符（串）替换成另一个字符(串）
void repalce_all_ditinct(string& str,const string& old_value,const string& new_value)
{
    for(string::size_type pos(0);pos!=string::npos;pos+=new_value.length()) 
    {
        if((pos=str.find(old_value,pos))!=string::npos)
            str.replace(pos,old_value.length(),new_value);
        else break;
    }
}