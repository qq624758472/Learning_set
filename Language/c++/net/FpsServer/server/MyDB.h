#ifndef _MYDB_H_
#define _MYDB_H_
#include <iostream>
#include <string>
#include <mysql/mysql.h>
using namespace std;

// sudo apt-get install libmysqlclient-dev
//新版本叫sudo apt-get install libmariadbclient-dev libmariadb-dev-compat

/**
 * @brief
ubuntu20.04安装并设置mariadb的密码
安装：
sudo apt install mariadb-server

第一次运行：(一定要用sudo)
sudo mysql

mysql > use mysql;
mysql > set password for ‘root’@‘localhost’ = password(‘123456’);
mysql > quit

设置完密码后一定要重启mysql服务：

systemctl restart mysql
 *
 */

class MyDB
{
private:
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;

public:
    MyDB();
    ~MyDB();
    bool initDb(string host, string user, string password, string db_name);
    bool execSql(string sql);
};
#endif
