# 数据库使用

## 环境安装(ubuntu16.04下)

1. ```shell
   
   需要安装的库
   yum install mysql-devel
   PyMySQL-0.8.1
   ```
   解压运行python setup.py install

2. 数据库和系统密码都是000000.

## 安装数据库

添加mysql仓库(-Uvh后面接的为你下载的rpm文件名)

```
sudo rpm -Uvh mysql57-community-release-el7-11.noarch.rpm
```

安装mysql

```mysql
sudo yum install -y mysql-community-server

启动mysql服务
sudo service mysqld start 
或
sudo systemctl start mysqld.service 

centos的mysql会使用默认密码，或者不用密码：
1.默认密码：
新版本的Mysql会为root用户创建一个初始密码，需要更改。
查看默认密码： sudo grep 'temporary password' /var/log/mysqld.log
2.若默认密码报错，可不用密码登录。
mysql -u root
SET PASSWORD FOR 'root'@'localhost' = PASSWORD('newpass');

第二句为修改密码，修改后就必须使用密码登录。
```

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

