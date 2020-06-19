#!/usr/bin/env python
# -*- coding: UTF-8 -*-




import os
import datetime
import write_datalib
import time,datetime
import threading  
import inspect  
import ctypes  

def run_cmd(cmd):
    if_return = os.system(cmd)
    if if_return != 0:
        return -1
    return 1


def read_file(path):
    with open(path, "r")as f:
        data=f.read()
    return data


def log(import_log):
    cmd = "echo %s   >> /home/pi/work/backup/backup.log" % import_log
    os.system(cmd)


#备份数据库类   
class back_datalib(object):
    def __init__(self, data_name,back_posion):
        self.data_name = data_name
        self.back_posion = back_posion

    #组合备份数据库命令
    def combination(self):
        back_posion = self.back_posion + "/" + self.data_name + ".sql"
        cmd = "mysqldump -h localhost -uroot -proot --opt %s > %s"%(self.data_name,back_posion)
        print "cmd is [%s]"%cmd
        if run_cmd(cmd) == 1:
            return True
        else:
            return False

#备份所有的数据库  如有任何错误，退出 。
def back_datalib_all(dir_time):
    #存在时，需要压缩数据库， /saming/   和  /samingData 文件夹
    ccnet = back_datalib("ccnet_db",dir_time)
    if ccnet.combination() == False:
        log("ccnet_db backup Error!")
        cmd = "rm " + dir_time + " -rf"
        run_cmd(cmd)
        exit()
    
    ccnet = back_datalib("seafile_db",dir_time)
    if ccnet.combination() == False:
        log("seafile_db backup Error!")
        cmd = "rm " + dir_time + " -rf"
        run_cmd(cmd)
        exit()
    
    ccnet = back_datalib("seahub_db",dir_time)
    if ccnet.combination() == False:
        log("seahub_db backup Error!")
        cmd = "rm " + dir_time + " -rf"
        run_cmd(cmd)
        exit()

def get_mount_posion():
    try:
        dir = read_file("/home/pi/work/conf/mount_posion.conf")
        return dir
    except:
        log("read_file[/home/pi/work/conf/mount_posion.conf] Error!")
        exit()
        return -1


def back_homecloud_all(dir_time,dir):
    if run_cmd("cd /saming") == -1:
        log("[cd /saming] is Error!")
    cmd = "tar -zcPvf " + dir_time + "/saming.tar.gz" + " /saming/*"
    if run_cmd(cmd) == -1:
        log("[%s] is Error!"%cmd)
        cmd = "rm " + dir_time + " -rf"
        run_cmd(cmd)
        return -1
    return 1


def back_data_all():
    if run_cmd("cd /samingData") == -1:
        log("[cd /samingData] is Error!")
    cmd = "tar -zcPvf " + dir_time + "/samingData.tar.gz" + " /samingData/seafile-data/*"
    if run_cmd(cmd) == -1:
        log("[%s] is Error!"%cmd)
        cmd = "rm " + dir_time + " -rf"
        run_cmd(cmd)
        return -1
    return 1

def calculate_schedule(dir):
    #计算进度条
    file_number = 0
    try:
        for root, dirs, files in os.walk(dir):
            file_number = file_number + len(files)
    except Exception as e:
        log(e)
        exit()
    i = float(45)/float(file_number) #一个文件所占百分比。
    # print "----->"  #0.0028返回后变成0了。
    # print i






def write_data(one_file,all_schedule,num,username):
    import time
    if one_file == None:
        one_file = 0.1
    while True:
        if all_schedule > num:
            all_schedule = num
            write_datalib.write_datalib(username,'12',int(all_schedule))
            return
        time.sleep(3)
        all_schedule = all_schedule + one_file
        write_datalib.write_datalib(username,'12',int(all_schedule))

#备份saming文件夹较快
def write_data_soft(one_file,all_schedule,num,username):
    import time
    if one_file == None:
        one_file = 0.5
    while True:
        time.sleep(3)
        all_schedule = all_schedule + one_file
        if all_schedule > num:
            all_schedule=num
            write_datalib.write_datalib(username,'12',int(all_schedule))
            return
        write_datalib.write_datalib(username,'12',int(all_schedule))


def py_get_time_dir():
    with open('/home/pi/work/conf/username.conf', 'r') as f:
        return f.read()
     


#备份功能：
#1.备份中，如果其中任何一个出错，就会退出并删除备份的文件夹，防止用户以为备份成功，然后恢复错误的备份导致系统崩溃。
#2.
if __name__=='__main__':
    pass