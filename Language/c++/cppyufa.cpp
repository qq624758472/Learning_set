

#include "cppYuFa.h"


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