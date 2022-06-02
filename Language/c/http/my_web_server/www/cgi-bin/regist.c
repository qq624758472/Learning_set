#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyDB.h"

//gcc regist.c MyDB.c -lmysqlclient -o regist

int sum_internal(char *query_str, char *num1, char *num2, int *success);
void sumnumbers(char *query_str);

int main(int argc, char **argv)
{
    sumnumbers(getenv("QUERY_STRING"));
    return 0;
}

void sumnumbers(char *query_str)
{
    int num1, num2, sum, success = 0;
    char passwd[64] = {0};
    char username[64] = {0};

    printf("HTTP/1.0 200 OK\r\n");
    printf("Content-type: text/html\r\n");
    printf("\r\n");
    if (sum_internal(query_str, username, passwd, &success) != 0)
    {
        printf("<h2>Invalid Numbers passed: %s</h2>", query_str);
    }
    else
    {
        printf("<h2>Regist success! <p></p>username=%s   passwd=%s</h2>\r\n",username,passwd);
    }
    printf("\r\n");

    MyDB db;
    memset(&db,0,sizeof(MyDB));
    db.myinit = MyDB_init;
    db.myrelease = MyDB_release;
    db.myconnect = initDb;
    db.myexec = execSql;

    db.myinit(&db);
    db.myconnect(&db,"127.0.0.1","root","000000","login");
    regist(&db,username,passwd);
    db.myrelease(&db);
}

int sum_internal(char *query_str, char *username, char *passwd, int *success)
{
    int pos;
    char *strpos = NULL;
    char str1[32], str2[32];
    memset(str1,0,32);
    memset(str2,0,32);

    if (query_str == NULL)
        return -1;

    strpos = strchr(query_str, '=');
    if (strpos == NULL)
        return -1;

    strpos += 1;

    int i=0;
    while((*strpos) != '&')
    {
        str1[i] = (*strpos);
        i++;
        strpos++;
    }

    strpos = strchr(strpos, '=');
    strpos+=1;
    i=0;
    while((*strpos) != 0 && (*strpos) != '\n' )
    {
        str2[i] = (*strpos);
        i++;
        strpos++;
    }

    memcpy(username,str1,strlen(str1));
    memcpy(passwd,str2,strlen(str2));

    return 0;
}
