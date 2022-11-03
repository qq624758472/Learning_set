#include<stdio.h>
extern int add(int ,int);




int main()
{
    int data1 = 10;
    int data2 = 20;
    int result = 0;
    result = add(data1,data2);
    //printf("the result is %d\n", result);
    return 0;
}