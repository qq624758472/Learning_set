#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MyDB.h"

// gcc MyDB.c -lmysqlclient -o regist

void MyDB_init(MyDB *db)
{
    db->conn = mysql_init(NULL);
    if (db->conn == NULL)
    {
        printf("error occurs %s\n", mysql_error(db->conn));
        exit(1);
    }
}

void MyDB_release(MyDB *db)
{
    if (db->conn != NULL)
    {
        mysql_close(db->conn);
    }
}

bool initDb(MyDB *db, char *host, char *user, char *password, char *db_name)
{
    db->conn = mysql_real_connect(db->conn, host, user, password, db_name, 0, NULL, 0);
    if (db->conn == NULL)
    {
        printf("error occurs %s\n", mysql_error(db->conn));
        exit(1);
    }
    return true;
}

bool regist(MyDB *db, char *username, char *passwd)
{
    char cmd[1024] = {0};
    sprintf(cmd, "insert into login (username,passwd) values (\"%s\",\"%s\");", username, passwd);
    if (execSql(db, cmd) == false)
    {
        printf("%s %d error\n", __FUNCTION__, __LINE__);
        return false;
    }

    return true;
}

bool select_username(MyDB *db, char *username, char *passwd)
{
    char cmd[1024] = {0};
    sprintf(cmd, "select passwd from login where username='%s';", username);
    //printf("cmd:%s\n",cmd);

    if (mysql_query(db->conn, cmd))
    {
        printf("error occurs %s\n", mysql_error(db->conn));
        return false;
    }
    else
    {
        db->result = mysql_store_result(db->conn);
        if (db->result)
        {
            //提取当前有多少条记录,这里可能会有条件限制，我需要那种数据
            int num = mysql_num_rows(db->result);
            if (num == 0) //认为没有记录
            {
                //printf("not fount\n");
                return false;
            }

            db->row = mysql_fetch_row(db->result);
            //计算列数
            int m = mysql_field_count(db->conn);
            //printf("一行记录有%d列\n", m);
            for (int j = 0; j < m; j++)
            {
                //printf("row[%d]=%s ", j, db->row[j]);
                if(memcmp(passwd, db->row[j], strlen(passwd)> strlen(db->row[j])? strlen(passwd): strlen(db->row[j]) ) == 0)
                {
                    mysql_free_result(db->result);
                    return true;
                }
            }
            printf("\n");
        }
        mysql_free_result(db->result);
    }
    return false;
}

bool execSql(MyDB *db, char *sql)
{
    if (mysql_query(db->conn, sql))
    {
        printf("error occurs %s\n", mysql_error(db->conn));
        return false;
    }
    else
    {
        db->result = mysql_use_result(db->conn);
        if (db->result)
        {
            int num_fields = mysql_num_fields(db->result); //
            int num_rows = mysql_field_count(db->conn);
            for (int i = 0; i < num_rows; i++)
            {
                db->row = mysql_fetch_row(db->result);
                if (db->row < 0 || db->row == NULL)
                    break;
                for (int j = 0; j < num_fields; j++)
                {
                    printf("%s\n", db->row[j]);
                }
                printf("\n");
            }
        }
        mysql_free_result(db->result);
    }
    return true;
}

//测试代码
#if 0
void main()
{
    MyDB db;
    memset(&db, 0, sizeof(MyDB));
    db.myinit = MyDB_init;
    db.myrelease = MyDB_release;
    db.myconnect = initDb;
    db.myexec = execSql;

    db.myinit(&db);
    db.myconnect(&db, "127.0.0.1", "root", "000000", "login");
    // db.myexec(&db,"select * from login");
    if(select_username(&db, "lll", "123") == true)
    {
        printf("找到了，密码正确\n");
    }
    else
    {
        printf("没找到用户名，或者密码不正确\n");
    }
    if(select_username(&db, "lll", "111") == true)
    {
        printf("找到了，密码正确\n");
    }
    else
    {
        printf("没找到用户名，或者密码不正确\n");
    }
    // regist(&db,"ssss","111");
    db.myrelease(&db);
}
#endif