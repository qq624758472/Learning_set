# 数据库使用

## 环境安装(ubuntu16.04下)

1. ```shell
   需要安装的库
   apt-get install mysql-server
   apt-get install mysql-client
   apt-get install libmysqlclient-dev
   ```

2. 数据库和系统密码都是000000.

## 设置数据库可以远程连接

1.登录：mysql -u root -h localhost -p

2.打开：use mysql 

3.\#将host设置为%表示任何ip都能连接mysql，当然您也可以将host指定为某个ip

​     update user set host='%' where user='root' and host='localhost';

4.flush privileges;        #刷新权限表，使配置生效

5.修改/etc/mysql/mysql.conf.d/mysqld.cnf文件。将bind_address=127.0.0.1该文0.0.0.0即可。

6.重启mysql   : /etc/init.d/mysql restart

## 关闭远程连接

如果您想关闭远程连接，恢复mysql的默认设置（只能本地连接），您可以通过以下步骤操作：

     use mysql                #打开mysql数据库
    
     #将host设置为localhost表示只能本地连接mysql
    
     update user set host='localhost' where user='root';
    
     flush privileges;        #刷新权限表，使配置生效

备注：您也可以添加一个用户名为yuancheng，密码为123456，权限为%（表示任意ip都能连接）的远程连接用户。命令参考如下：

     grant all on *.* to 'yuancheng'@'%' identified by '123456';
    
     flush privileges;

## 数据库操作命令

###### 1.创建数据库

```mysql
CREATE DATABASE 数据库名;
```

###### 2.删除数据库

```mysql
drop database <数据库名>;
```

###### 3.选择数据库

```mysql
use RUNOOB;
```

### 增

```mysql
INSERT INTO table_name ( field1, field2,...fieldN )
                       VALUES
                       ( value1, value2,...valueN );
```

### 删

### 改

### 查

```mysql
SELECT column_name,column_name
FROM table_name
[WHERE Clause]
[LIMIT N][ OFFSET M]
```

## 管理数据库命令

- **USE 数据库名** :
  选择要操作的Mysql数据库，使用该命令后所有Mysql命令都只针对该数据库。
  
  ```mysql
  mysql> use RUNOOB;
  Database changed
  ```

- **SHOW DATABASES:**
  列出 MySQL 数据库管理系统的数据库列表。
  
  ```mysql
  mysql> SHOW DATABASES;
  +--------------------+
  | Database           |
  +--------------------+
  | information_schema |
  | RUNOOB             |
  | cdcol              |
  | mysql              |
  | onethink           |
  | performance_schema |
  | phpmyadmin         |
  | test               |
  | wecenter           |
  | wordpress          |
  +--------------------+
  10 rows in set (0.02 sec)
  ```

- **SHOW TABLES:**
  显示指定数据库的所有表，使用该命令前需要使用 use 命令来选择要操作的数据库。
  
  ```mysql
  mysql> use RUNOOB;
  Database changed
  mysql> SHOW TABLES;
  +------------------+
  | Tables_in_runoob |
  +------------------+
  | employee_tbl     |
  | runoob_tbl       |
  | tcount_tbl       |
  +------------------+
  3 rows in set (0.00 sec)
  ```

- **SHOW COLUMNS FROM 数据表:**
  显示数据表的属性，属性类型，主键信息 ，是否为 NULL，默认值等其他信息。
  
  ```mysql
  mysql> SHOW COLUMNS FROM runoob_tbl;
  +-----------------+--------------+------+-----+---------+-------+
  | Field           | Type         | Null | Key | Default | Extra |
  +-----------------+--------------+------+-----+---------+-------+
  | runoob_id       | int(11)      | NO   | PRI | NULL    |       |
  | runoob_title    | varchar(255) | YES  |     | NULL    |       |
  | runoob_author   | varchar(255) | YES  |     | NULL    |       |
  | submission_date | date         | YES  |     | NULL    |       |
  +-----------------+--------------+------+-----+---------+-------+
  4 rows in set (0.01 sec)
  ```

- **SHOW INDEX FROM 数据表:**
  显示数据表的详细索引信息，包括PRIMARY KEY（主键）。
  
  ```
  mysql> SHOW INDEX FROM runoob_tbl;
  +------------+------------+----------+--------------+-------------+-----------+-------------+----------+--------+------+------------+---------+---------------+
  | Table      | Non_unique | Key_name | Seq_in_index | Column_name | Collation | Cardinality | Sub_part | Packed | Null | Index_type | Comment | Index_comment |
  +------------+------------+----------+--------------+-------------+-----------+-------------+----------+--------+------+------------+---------+---------------+
  | runoob_tbl |          0 | PRIMARY  |            1 | runoob_id   | A         |           2 |     NULL | NULL   |      | BTREE      |         |               |
  +------------+------------+----------+--------------+-------------+-----------+-------------+----------+--------+------+------------+---------+---------------+
  1 row in set (0.00 sec)
  ```

- **SHOW TABLE STATUS LIKE [FROM db_name] [LIKE 'pattern'] \G:**
  该命令将输出Mysql数据库管理系统的性能及统计信息。
  
  ```mysql
  mysql> SHOW TABLE STATUS  FROM RUNOOB;   # 显示数据库 RUNOOB 中所有表的信息
  
  mysql> SHOW TABLE STATUS from RUNOOB LIKE 'runoob%';     # 表名以runoob开头的表的信息
  mysql> SHOW TABLE STATUS from RUNOOB LIKE 'runoob%'\G;   # 加上 \G，查询结果按列打印
  ```

# 数据库表的设计

数据库名： suiver_game

## login表

登录的用户表

| id  | username | passwd |
| --- | -------- | ------ |
| 1   | liu      | 111    |

## goodsInfo表

这个表保存所有的物品，物品分类：1.装备 2.药品 3.兑换物品 4.基础材料               

| id   | name | type | blood | magic | attack | speed  | duration |
| ---- | ---- | ---- | ----- | ----- | ------ | ------ | -------- |
| 物品id | 物品名称 | 物品类型 | 加的血量  | 加的蓝量  | 加的攻击力  | 加的移动速度 | 效果持续时间   |

duration： 血瓶为永久加血。持续时间为0.永久  -1.装备   >0.持续的秒数。

## charactInfo表

角色登录完成后发送的角色信息。

| id   | posion |
| ---- | ------ |
| 角色id | 角色位置   |
