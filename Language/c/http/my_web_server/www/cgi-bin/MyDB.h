#ifndef _MYDB_H_
#define _MYDB_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>


#ifndef bool
#define bool int
#define true 1
#define false 0
#endif

typedef void (*init)(void *);
typedef void (*release)(void *);
typedef bool (*dbconnect)(void *, char *, char *, char *, char *);
typedef bool (*dbexec)(void *, char *);

// sudo apt-get install libmysqlclient-dev
//新版本叫sudo apt-get install libmariadbclient-dev libmariadb-dev-compat
typedef struct _MyDB
{
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;

    init myinit;
    release myrelease;
    dbconnect myconnect;
    dbexec myexec;
}MyDB;



void MyDB_init(MyDB *db);
void MyDB_release(MyDB *db);
bool execSql(MyDB *db, char *sql);
bool initDb(MyDB *db, char *host, char *user, char *password, char *db_name);
#endif
