/*
 * @Description: 
 * @Version: 2.0
 * @Autor: lsh
 * @Date: 2021-10-18 13:23:50
 * @LastEditors: lsh
 * @LastEditTime: 2021-10-18 16:32:31
 */


#include "test_c.h"

static void arm_cpu_register_types(void)
{
    printf("%s,%d\n",__FUNCTION__,__LINE__);
}


/**
 * __attribute__实验
*/
type_init(arm_cpu_register_types);

/**
 * setbit宏定义解析实验
*/
static char *inode_map;
#define CHAR_BIT  __CHAR_BIT__  //获得一个字符对象的比特的总数
#define NBBY		CHAR_BIT
#define setbit(a,i)     ((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define mark_inode(x) (setbit(inode_map, (x)))

int main()
{
    inode_map = malloc(1 * 1024);
    mark_inode(2);
    pause();
}