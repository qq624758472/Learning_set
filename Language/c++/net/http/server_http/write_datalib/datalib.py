#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import os
import datetime
import write_datalib
import time
import threading  
import inspect  
import ctypes  
import json

def run_cmd(cmd):
    if_return = os.system(cmd)
    if if_return != 0:
        return -1
    return 1


def read_file(path):
    with open(path, "r")as f:
        data=f.read()
    return data


def log(revert_log):
    cmd = "echo %s   > /home/pi/work/write_datalib/datalib.log" % revert_log
    os.system(cmd)


def log_init():
    cmd = "echo Start revert!   > /home/pi/work/write_datalib/datalib.log"
    os.system(cmd)
    return 1

def revert_data(revert_back_path):
    ccnet_path = revert_back_path + "/samingData.tar.gz"
    cmd = "tar -zxPvf %s"%ccnet_path
    print cmd
    if run_cmd(cmd) == -1:
        return -1
    return 1


def revert_datalib(revert_back_path):
    ccnet_path = revert_back_path + "/ccnet_db.sql"
    cmd = "mysql -uroot -proot ccnet_db < %s"%ccnet_path
    print cmd
    if run_cmd(cmd) == -1:
        return -1

    seafile_path = revert_back_path + "/seafile_db.sql"
    cmd = "mysql -uroot -proot seafile_db < %s"%seafile_path
    print cmd
    if run_cmd(cmd)== -1:
        return -1

    seahub_path = revert_back_path + "/seahub_db.sql"
    cmd = "mysql -uroot -proot seahub_db < %s"%seahub_path
    print cmd
    if run_cmd(cmd) == -1:
        return -1
    return 1

#还原数据库较快。
def write_data(all_schedule,num,username):
    import time
    one_file = 1.0
    while True:
        time.sleep(1)
        all_schedule = all_schedule + one_file
        if all_schedule > num:
            all_schedule=num
        write_datalib.write_datalib(username,'12',int(all_schedule))

#还原samingData文件夹较慢
def write_data_soft(all_schedule,num,username):
    import time
    one_file = 0.5
    while True:
        time.sleep(3)
        all_schedule = all_schedule + one_file
        if all_schedule > num:
            all_schedule=num
        write_datalib.write_datalib(username,'12',int(all_schedule))

def _async_raise(tid, exctype):  
    """raises the exception, performs cleanup if needed"""  
    tid = ctypes.c_long(tid)  
    if not inspect.isclass(exctype):  
        exctype = type(exctype)  
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, ctypes.py_object(exctype))  
    if res == 0:  
        raise ValueError("invalid thread id")  
    elif res != 1:  
        # """if it returns a number greater than one, you're in trouble,  
        # and you should call it again with exc=NULL to revert the effect"""  
        ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)  
        raise SystemError("PyThreadState_SetAsyncExc failed")  
    
def stop_thread(thread):  
    _async_raise(thread.ident, SystemExit)  


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



def get_operating_database(username):
    list_history=write_datalib.get_operating_info(username)
    dic = {'datalib_history':list_history}
    json_data=json.dumps(dic)
    return json_data


#还原功能：
if __name__=='__main__':
    # username = 'hao'
    # print get_operating_database(username)
    pass
    # share_or_person=1
    # mount_posion='/dev/sda1'
    # time  = datetime.datetime.now().strftime('%Y%m%d %H:%M:%S')
    # print time
    # write_datalib.write_operating_history_datalib("2229",'13',-1,time,share_or_person,mount_posion)


    # #获取谁在使用互拷功能。
    # try:
    #     file = open("/home/pi/work/conf/username.conf",'r')
    # except IOError:
    #     log("open /home/pi/work/conf/passwd.conf Error!")
    #     exit()
    # username=file.read()   #用户名
    # file.close()

    # list_history=write_datalib.get_operating_info(username)
    # #备份的数据存在，还原备份的数据。
    # print list_history
    # fo = open("/home/pi/work/conf/datalib_history.conf", "w") #覆盖的方式打开。
    # if fo==0:
    #     print('open [/home/pi/work/conf/datalib_history.conf] Error!')
    #     log('open [/home/pi/work/conf/datalib_history.conf] Error!')
    #     exit()
    # dic = {'datalib_history':list_history}
    # json_data=json.dumps(dic)
    # fo.write(json_data)
    # fo.close()