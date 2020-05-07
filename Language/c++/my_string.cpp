#include "my_string.h"

inline int add(int x)
{
	cout << x << endl;
	return 0;
}

int test_inline()
{
	cout << "liushihao " "is good" << endl;
	cout << "liushihao "
		"is good" << endl;
	add(6);
	return 0;
}

//字符串拼接。
/*
data()以字符数组的形式返回字符串内容，但并不添加’\0’。
c_str()返回一个以‘\0’结尾的字符数组，
copy()则把字符串的内容复制或写入既有的c_string或字符数组内。
C++字符串并不以’\0’结尾。我的建议是在程序中能使用C++字符串就使用，
除非万不得已不选用c_string
*/
int test_string()
{
	string Str("/api2/repos/");
	const char *token = "ca10ba0d-60c3-4a44-a439-e7d6a9de7d04";
	
	Str = Str + token + "/" + "?p=/%E9%9F%B3%E4%B9%90";
	cout << "Str:" << Str.data() << endl;  
	cout << "Str:" << Str.c_str() << endl; 
	return 0;
}
