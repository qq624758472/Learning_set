#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：login.py
 
#import socket               # 导入 socket 模块
import pymysql              #

class login():
    def __init__(self):
        # 连接database
        self.conn = pymysql.connect(host="127.0.0.1", user="root",password="000000",database="suiver_game",charset="utf8")
        # 得到一个可以执行SQL语句的光标对象
        self.cursor = self.conn.cursor()
        # 定义要执行的SQL语句
    def connect(self):  #判断数据库是否连接成功。
        sql = """
        SELECT username
        FROM login
        """
        # 执行SQL语句
        ret = self.cursor.execute(sql)          #查询到的结果数量
        print ("{} rows in set.".format(ret))
        # #取一条
        # ret1 = cursor.fetchone()
        # #取三条
        # ret2 = cursor.fetchmany(3)
        #取全部
        #ret3 = cursor.fetchall()
        return ret
    def cha(self,name,passwd):
        value = False
        # 2, 执行sql语句
        sql='select passwd from login where username =\'%s\''%name
        print (sql)
        self.cursor.execute(sql)
        result=self.cursor.fetchone()
        #print "result: %s ,result[0]:%s"%(result,result[0])  #result[0] 去掉多余字符
        if result==None:
            print("用户名错误，登录失败")
        elif result[0]==passwd:
            print("登录成功")
            value = True
        else:
            print("密码错误，登录失败")
        self.release()  #释放资源
        return value

    def zeng(self, name, passwd):
        value = False
        sql='SELECT count(*) FROM login'
        print (sql)
        self.cursor.execute(sql)
        result=self.cursor.fetchone()
        #print "result: %s ,result[0]:%s"%(result,result[0])  #result[0] 去掉多余字符
        if result==None:
            print("数据库出现错误")
        else:
            value = True
            sql='INSERT INTO login (id,username,passwd) VALUES (\'%s\',\'%s\',\'%s\')'%(result[0]+1,name,passwd)
            print (sql)
            self.cursor.execute(sql)
            self.conn.commit()                   #数据库进行命令提交            # 提交所有的操作，如果之前已经执行多次的execute，那么就都进行提交
        self.release()  #释放资源
        return value

    def isExist(self, username): #判断用户名是否存在
        value = False
        # 2, 执行sql语句
        sql='select passwd from login where username =\'%s\''%username
        print (sql)
        self.cursor.execute(sql)
        result=self.cursor.fetchone()
        #print "result: %s ,result[0]:%s"%(result,result[0])  #result[0] 去掉多余字符
        if result==None:
            print("用户名不存在，可以注册")
            value = True
        else:
            print('注册：该用户名存在，注册失败')
        return value

    def release(self):
        # 关闭光标对象
        self.cursor.close()
        # 关闭数据库连接
        self.conn.close()

def clientLogin(username, passwd):
    t = login()
    if t.connect() > 0:
        print ("连接成功")
    else:
        print ("连接失败")
        return False
    return t.cha(username,passwd)

def clientRegist(username, passwd):
    value = False
    t = login()
    if t.connect() > 0:
        print ("连接成功")
    else:
        print ('连接失败')
        return False
    if t.isExist(username) == True:    #用户名不存在，可以注册
        value = t.zeng(username,passwd)
    return value

# if __name__=='__main__':
    

    
