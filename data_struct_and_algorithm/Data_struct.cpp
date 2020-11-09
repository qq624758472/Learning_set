#include "Data_struct.h"
#include <time.h>
#include <stdlib.h>

DirectInsert::DirectInsert()
{
    for(int i = 0; i<10; i++)
    {
        L[i] = GetRandomNumber();
        cout << "L[" << i << "]= "<< L[i]  << " ";
    }
}
DirectInsert::~DirectInsert()
{

}

/*
直接插入排序   
*/
listNode * DirectInsert::directInsert_list(listNode *L)
{
}

/*
直接插入排序：L是10个无序数组。   L[0]
*/
int * DirectInsert::directInsert_int(int L[10])
{

}



int GetRandomNumber()
{
	int RandomNumber;
	//srand((unsigned)time(NULL));//time()用系统时间初始化种。为rand()生成不同的随机种子。
    //srand(NULL);
	RandomNumber = rand() % 100 + 1;//生成1~100随机数
	return RandomNumber;
}
