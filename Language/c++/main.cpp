#include "stream.h"
#include "sort.h"
#include "my_string.h"
#include "p_class.h"
#include "template.h"
#include "cppYuFa.h"
using namespace std;

/*
	这是一个C++学习的工程，所有练手项目的集合。
*/

//在编译的时候，就会连接再一起，如果连在一起找不到该名称，编译就会报错。
#define concat(a, b) a ## b   

//在编译的时候，就会把 replacement-text 令牌转换为用引号引起来的字符串，替换后找不到该名称，编译就会报错。
#define MKSTR( x ) #x       
 
#define DEBUG


int main ()
{
    //      1文件操作类
#ifdef DEBUG1
    FileWrite write;
    write.operating();

    FileRead read;
    read.operating();
#endif    

    //      2类的构造函数
#ifdef DEBUG2
    //Sort sort(8,4);
    Sort sort;
    try
    {
        cout << sort.division() << endl;
    }
    catch(const char* msg){
        cerr << msg << endl;
    }
#endif  

    //      3两种宏定义
#ifdef DEBUG3
    int xy = 100;
    cout << concat(x, y) << endl;

    cout << MKSTR(liu shi hao) << endl;
#endif

    //      4条件编译
#ifdef DEBUG4
    cerr <<"Variable x " << endl;
#endif

    //      5NULL的判断
#ifdef DEBUG5
    char * response = NULL;
    if(response)
    {
        cout << "NULL" << endl;
    }
#endif

    //      6测试内联函数
#ifdef DEBUG6
    test_inline();
#endif

    //      7测试类的指针, 子类初始化父类。
#ifdef DEBUG7
    //p_class();
    const_class();
#endif

    //      8测试静态局部变量
#ifdef DEBUG8
    value();
#endif

    //      9指向函数的指针
#ifdef DEBUG9
    p_hanshu();
#endif


#ifdef DEBUG10
    test_string();
#endif


#ifdef DEBUG11
	CppYuFa test;
	for (int i = 0; i < 161; i++) {
		test.putDeque((char*)"1");
	}
	cout << test.seqImages.size() << endl;
#endif

#ifdef DEBUG12
	//使用平台判断
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

#ifdef DEBUG
		//
#endif
	getchar();
    return 0;
}

 
