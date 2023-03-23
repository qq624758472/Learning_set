#include "stream.h"
#include "sort.h"
#include "my_string.h"
#include "p_class.h"
#include "template.h"
#include "cppYuFa.h"
#include "exam.h"
#include "DuoTai.h"
#include "common.h"
using namespace std;

/*
	这是一个C++学习的工程，所有练手项目的集合。
*/

//在编译的时候，就会连接再一起，如果连在一起找不到该名称，编译就会报错。
#define concat(a, b) a##b

//在编译的时候，就会把 replacement-text 令牌转换为用引号引起来的字符串，替换后找不到该名称，编译就会报错。
#define MKSTR(x) #x

#define DEBUG

int main(int argc,char *argv[])
{
    //1.文件操作类
#ifdef DEBUG1
    FileWrite write;
    write.operating();

    FileRead read;
    read.operating();
#endif

    //2.类的构造函数
#ifdef DEBUG2
    //Sort sort(8,4);
    Sort sort;
    try
    {
        cout << sort.division() << endl;
    }
    catch (const char *msg)
    {
        cerr << msg << endl;
    }
#endif

    //3.两种宏定义
#ifdef DEBUG3
    int xy = 100;
    cout << concat(x, y) << endl;

    cout << MKSTR(liu shi hao) << endl;
#endif

    //4.条件编译
#ifdef DEBUG4
    cerr << "Variable x " << endl;
#endif

    //5.NULL的判断
#ifdef DEBUG5
    char *response = NULL;
    if (response)
    {
        cout << "NULL" << endl;
    }
#endif

    //6.测试内联函数
#ifdef DEBUG6
    test_inline();
#endif

    //7.测试类的指针, 子类初始化父类。
#ifdef DEBUG7
    //p_class();
    const_class();
#endif

    //8.测试静态局部变量
#ifdef DEBUG8
    value();
#endif

    //9.指向函数的指针
#ifdef DEBUG9
    p_hanshu();
#endif

    //10.字符串测试
#ifdef DEBUG10
    test_string();
#endif

    //11.c艹语法
#ifdef DEBUG
    CppYuFa test;
    for (int i = 0; i < 161; i++)
    {
        test.putDeque((char *)"1");
    }
    cout << test.seqImages.size() << endl;
#endif

//使用平台判断
#ifdef DEBUG12
    #ifdef WIN32
        cout << "I'm windows" << endl;
    #endif

    #ifdef linux
        cout << "I'm linux" << endl;
    #endif

    //使用编译器判断
    #ifdef _MSC_VER
        cout << "I'm windows" << endl;
    #endif

    #ifdef __GNUC__
        cout << "I'm linux" << endl;
    #endif
#endif

//实现一个desktop文件解析工具
#ifdef DEBUG13
    //if
    if(argc < 3)
    {
        cout << "arg error" << endl;    
        return 1;
    }


//define 
    int status = 1;
    vector<string> arg;
    string path; 
    FileOperation file(path);

//init
    for(int i=1;i<argc; i++)
    {
        arg.push_back(argv[i]);
    }


//pro
    //查看主函数参数
    vector<string>::iterator v = arg.begin();
    while(v != arg.end())
    {
#ifdef DEBUG_EXAM
        cout << "arg is: "<< *v << endl;
#endif
        if(*v == "-d")
        {
            v++;
            path = *v;
#ifdef DEBUG_EXAM
            cout << "path is:" << path << endl;
#endif
            file.updatePath(path);
            if(!file.isExist()) //文件是否存在
            {
                status = 1;
                cout << "文件不存在" << endl;
                goto error;
            }

            //判断文件内容格式是否正确.
            if(!file.isFormat()) 
            {
                status = 1;
                cout << "文件格式不正确" << endl;
                goto error;
            }
        }

        if(*v == "-n") //获取应用程序名称
        {
            if(!file.isPathInit())
            {
                cout << "请将-d参数放到最前" << endl;
                status = 1;
                goto error;
            }
            //将应用程序名称打印出来
            {
                vector<string>::iterator v = file.name.begin();
                while(v != file.name.end())
                {
                    cout <<  *v << endl;
                    v++;
                }
            }
        }

        if(*v == "-i") //获取应用程序的图标
        {
            if(!file.isPathInit())
            {
                cout << "请将-d参数放到最前" << endl;
                status = 1;
                goto error;
            }
            //将图标打印出来
            {
                vector<string>::iterator v = file.icon.begin();
                while(v != file.icon.end())
                {
                    cout << *v << endl;
                    v++;
                }
            }
        }

        if(*v == "-e") //执行应用程序
        {
            if(!file.isPathInit())
            {
                cout << "请将-d参数放到最前" << endl;
                status = 1;
                goto error;
            }
            //取出-e后边的所有参数,遇到-d 或 -i 或 -n 后结束.  
            string runArg, runExec, cmd, target = "%f"; //保存所有-e的参数
            v++;
            while (v != arg.end())
            {
                if (*v == "-d" || *v == "-i" || *v == "-n")
                {
                    v--;   
                    break;
                }
                //runArg.push_back(*v);
                runArg += *v;
                runArg += " ";
                v++;
            }          
            {
                vector<string>::iterator v = file.exec.begin();
                while (v != file.exec.end())
                {
                    runExec = *v;
                    repalce_all_ditinct(runExec,target,runArg);
                    cmd =runExec;
#ifdef DEBUG_EXAM
                    cout << "cmd:" << cmd << endl;
#endif
                    system(cmd.c_str());//执行程序
                    v++;
                }
            }            
        }

        if(v != arg.end()) v++;
    }

    

success:
    return status;
error:
#ifdef DEBUG_EXAM
    cout << "error exit" << endl;
#endif


    return status;
#endif



#ifdef DEBUG14
    char tmp[100] = {0};
    memset(tmp,0,sizeof(tmp));
    change(tmp);
    cout << tmp <<endl;
#endif

#ifdef DEBUG15
    //多态测试
    duoTaiTest();
#endif


    //getchar();
    return 0;
}
