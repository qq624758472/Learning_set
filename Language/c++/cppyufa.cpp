

#include "cppYuFa.h"
#include <cstring>

void CppYuFa::putDeque(char* que)
{
	if (seqImages.size() < frameNumCache)
	{
		seqImages.push_back(que);
	}
	else
	{
		char *tmp = seqImages.front();  //起始元素。
		seqImages.pop_front();//删除队列第一个元素。
		assert(seqImages.size() < frameNumCache);
		seqImages.push_back(que);
	}
}


ConstTest::ConstTest()
{
    memset(buff, 0, 4096);
}

bool ConstTest::test()
{
    ptr1 = buff;
    //ptr1[0] = 'l';//编译会报错，因为不允许修改。
//=================================================
    //ptr3 = buff;//该指针不可修改，定义时必须赋值.
    ptr3[0] = '1';
    
    return true;
}