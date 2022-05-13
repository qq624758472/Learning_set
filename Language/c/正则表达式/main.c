/*
 * @Description:正则表达式,linux下c语言用法
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-04-08 21:44:54
 * @LastEditors: ruog__
 * @LastEditTime: 2022-04-14 15:47:56
 */
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
int my_match(char *pattern, char *buf)
{
    int status, i;
    int flag = REG_EXTENDED;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regex_t reg;
    //编译正则模式
    regcomp(&reg, pattern, flag);
    //执行正则表达式和缓存的比较
    status = regexec(&reg, buf, nmatch, pmatch, 0);
    //打印匹配的字符串
    for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i)
    {
        putchar(buf[i]);
    }
    printf("\n");
    regfree(&reg);
    return status;
}

int main()
{
    char pattern[1024] = "e(.*)o";
    char buf[1024] = "Hello World";
    int status = my_match(pattern, buf);
    if (status == REG_NOMATCH)
        printf("No match!\n");
    else if (0 == status)
    {
        printf("匹配成功！\n");
    }
    return 0;
}
