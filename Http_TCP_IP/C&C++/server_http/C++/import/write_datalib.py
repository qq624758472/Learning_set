#!/usr/bin/python
# -*- coding: UTF-8 -*-

import MySQLdb
import os

def run_cmd(cmd):
    if_return = os.system(cmd)
    if if_return != 0:
        return -1
    return 1


class Database:
    def _init_(self):
        self.db=None
        self.cursor=None
        pass
    def open_datalib(self ,ip,usrname,passwd,db_name):
        # 打开数据库连接
        try:
            self.db = MySQLdb.connect(ip, usrname, passwd, db_name, charset='utf8' )
            self.cursor=self.db.cursor()
            return True
        except Exception as e:
            print e
            return False
    
    def create_datalib(self):
        cmd = "/home/pi/work/create_history_mysql.sh"   #创建数据库脚本所在的路径。
        if run_cmd(cmd) == -1:
            return -1
        cmd = "/home/pi/work/create_mysql.sh"   #创建数据库脚本所在的路径。
        if run_cmd(cmd) == -1:
            return -1
        return 1

#导入导出 备份还原四个功能模块的  1.操作人 2.啥操作   3.百分比1-100的数字
#数据库名：operating_info    表名：operating_info   
# 字段：operating_person   operating_name    operating_schedule
    def query_sql(self,sql):
        if sql != None:
            try:
                self.cursor.execute(sql)
                result_list=self.cursor.fetchall()
                return result_list
            except Exception as e:
                print e
                return 


    def change_sql(self,sql):
        if sql != None:
            try:
                self.cursor.execute(sql)
                self.db.commit()
                return True
            except Exception as e:
                print e
                self.db.rollback()
                return False

    def db_close(self):
        self.db.close()
        self.cursor.close()


def write_datalib(username,source,target,operation,schedule):
    #写数据库操作
    HOSTNAME="localhost"
    USERNAME="root"
    PASSWORD="root"
    DBNAME="operating_info"
    TABLENAME="operating_info"

    operating_person="operating_person"    #谁在进行操作。    homecloud@saming.com
    operating_source='operating_source'
    operating_target='operating_target'
    operating_name= "operating_name"           #进行的什么操作    command: 00.无操作    10.import   11.export   12.backup   13.revert
    operating_schedule="operating_schedule"     #操作的进度     1到100的int 型数

    # username = "homecloud@saming.com"
    # operation = "13"
    # schedule = 55

    sql="insert into %s (%s,%s,%s,%s,%s)values('%s','%s','%s','%s','%d');"%(DBNAME,operating_person,operating_source,operating_target,operating_name,operating_schedule,username,source,target,operation,schedule)
    sql_empty="delete from %s"%(TABLENAME)

    print sql
    my_db= Database()
    bool_link=my_db.open_datalib("localhost","root","root","operating_info") 
    #数据库存在，表存在，然后开始清表，开始写入数据。
    if bool_link:  
        result_list= my_db.change_sql(sql_empty)
        result_list= my_db.change_sql(sql)
        my_db.db_close()
    else:
        #数据库不存在，则创建数据库，创建表。
        print "datalib not exists ,start create!"
        if my_db.create_datalib() == -1:
            print "create Error!"
            return -1
        #创建成功
        bool_link=my_db.open_datalib("localhost","root","root","operating_info") 
        if bool_link: 
            result_list= my_db.change_sql(sql_empty)
            result_list= my_db.change_sql(sql)
            my_db.db_close()
        else:
            print "Create datalib Error!"
            return -1



def write_operating_history_datalib(username,operation,schedule,time,source,target):
    #写数据库操作
    HOSTNAME="localhost"
    USERNAME="root"
    PASSWORD="root"
    DBNAME="operating_info"
    TABLENAME="operating_history_info"

    operating_person="operating_person"    #谁在进行操作。    homecloud@saming.com
    operating_name= "operating_name"           #进行的什么操作    command: 00.无操作    10.import   11.export   12.backup   13.revert
    operating_schedule="operating_schedule"     #操作的进度     1到100的int 型数
    operating_time='operating_time'
    operating_source='operating_source'
    operating_target='operating_target'
    # username = "homecloud@saming.com"
    # operation = "13"
    # schedule = 55

    sql="insert into %s (%s,%s,%s,%s,%s,%s)values('%s','%s','%s','%s','%s','%d');"%(TABLENAME,operating_person,operating_name,operating_time,operating_source,operating_target,operating_schedule,username,operation,time,source,target,schedule)
    sql_empty="delete from %s"%(TABLENAME)
    print sql
    my_db= Database()
    bool_link=my_db.open_datalib("localhost","root","root",DBNAME)
    #数据库存在，表存在，然后开始清表，开始写入数据。
    if bool_link:  
        #result_list= my_db.change_sql(sql_empty)
        result_list= my_db.change_sql(sql)
        my_db.db_close()
    else:
        #数据库不存在，则创建数据库，创建表。
        print "datalib not exists ,start create!"
        if my_db.create_datalib() == -1:
            print "create Error!"
            return -1
        #创建成功
        bool_link=my_db.open_datalib("localhost","root","root",DBNAME) 
        if bool_link: 
            #result_list= my_db.change_sql(sql_empty)
            result_list= my_db.change_sql(sql)
            my_db.db_close()
        else:
            print "Create datalib Error!"
            return -1


def empty_operating_history_datalib(username):
    #写数据库操作
    HOSTNAME="localhost"
    USERNAME="root"
    PASSWORD="root"
    DBNAME="operating_info"
    TABLENAME="operating_history_info"

    operating_person="operating_person"    #谁在进行操作。    homecloud@saming.com
    operating_name= "operating_name"           #进行的什么操作    command: 00.无操作    10.import   11.export   12.backup   13.revert
    operating_schedule="operating_schedule"     #操作的进度     1到100的int 型数
    operating_time='operating_time'
    operating_source='operating_source'
    operating_target='operating_target'


    sql_empty= "delete from %s where operating_person='%s'"%(TABLENAME,username)
    my_db= Database()
    bool_link=my_db.open_datalib("localhost","root","root",DBNAME)
    #数据库存在，表存在，然后开始清表，开始写入数据。
    if bool_link:  
        result_list= my_db.change_sql(sql_empty)
        my_db.db_close()
        return 1
    else:
        #数据库不存在
        print "datalib not exists ,start create!"  
        return -1



#Warning:json返回的数据库历史记录最多只能获取76条
def get_operating_info(username):
    HOSTNAME="localhost"
    USERNAME="root"
    PASSWORD="root"
    DBNAME="operating_info"
    TABLENAME="operating_history_info"

    list_history = []

    operating_person="operating_person"    #谁在进行操作。    homecloud@saming.com
    operating_name= "operating_name"           #进行的什么操作    command: 00.无操作    10.import   11.export   12.backup   13.revert
    operating_schedule="operating_schedule"     #操作的进度     1到100的int 型数
    
    my_db= Database()
    sql="select * from %s where operating_person='%s'"%(TABLENAME,username)
    bool_link=my_db.open_datalib("localhost","root","root","operating_info") 
    if bool_link:
        result_list= my_db.query_sql(sql)
        if len(result_list) <= 0:   #数据库中有几条记录
            #log("datalib is Error! info is [%d]"%len(result_list))
            my_db.db_close()
            return -1
        if len(result_list) >= 70:
            sql="select * from %s where operating_person='%s' limit 70"%(TABLENAME,username)
            result_list= my_db.query_sql(sql)
        for line in result_list:
            #print("0 is [%s]"%line[0])
            #print("1 is [%s]"%line[1])
            #print("2 is [%s]"%line[2])
            operating_person          = line[0]
            operating_name            = line[1]
            operating_time            = line[2]
            operating_share_or_person = line[3]
            operating_mount_posion    = line[4]
            operating_schedule        = line[5]
            operating_info_result = {'operating_person':operating_person,
                                    'operating_name':operating_name, 
                                    'operating_schedule':operating_schedule,
                                    'operating_time':operating_time,
                                    'operating_share_or_person':operating_share_or_person,
                                    'operating_mount_posion':operating_mount_posion
                                    }
            list_history.append(operating_info_result)
        my_db.db_close()
    else:
        return -1
    return list_history
    




if __name__ == '__main__':
    pass
    
    
   
    
 