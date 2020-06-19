#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import os
import datetime
import write_datalib
import time
import threading  
import inspect  
import ctypes  
import simplejson as json

backup_source = 'null'
backup_target = '/samingData'

#用来保存上一次进度
last_schedule = 0


def read_json_data(mount_posion):
    with open("/home/pi/work/disk-init/diskinfo.conf", "r")as f:
        data=f.read()
    json_data=json.loads(data)['DiskInfo']
    if json_data == None:
        print 'json_data is NULL!'
        return -1
    for disk in json_data:
       dir = ''.join(disk["mount_posion"].split())
       #print dir
       if dir == mount_posion:
           return ''.join(disk["dev_name"].split())
    return -1



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
    cmd = "echo %s   >> /home/pi/work/revert/revert.log" % revert_log
    os.system(cmd)


def log_init():
    cmd = "echo Start revert!   > /home/pi/work/revert/revert.log"
    os.system(cmd)
    return 1

# def revert_data(revert_back_path):
#     ccnet_path = revert_back_path + "/samingData.tar.gz"
#     cmd = "tar -zxPvf %s"%ccnet_path
#     print cmd
#     if run_cmd(cmd) == -1:
#         return -1
#     return 1

def translate_time(time):
    year = time[0:4]
    mon = time[4:6]
    day = time[6:8]
    shi = time[8:10]
    fen = time[10:12]
    miao = time[12:14]
    #tmp = '/increments.'+year+'-'+mon+'-'+day+'T'+shi+':'+fen+':'+miao+'+'+'08:00.dir'
    tmp = year+'-'+mon+'-'+day+'T'+shi+':'+fen+':'+miao
    return tmp

def revert_data(revert_back_path):
    dir_time = revert_back_path.split('/')[-1]
    time = translate_time(dir_time)
    dir_path = revert_back_path.split('/')[0]+'/'+revert_back_path.split('/')[1]+'/'+revert_back_path.split('/')[2]+'/'+revert_back_path.split('/')[3]+'/'+revert_back_path.split('/')[4]+'/'+revert_back_path.split('/')[5]+'/'+revert_back_path.split('/')[6]+'/backupdata'
    
    cmd = " rdiff-backup -r %s --force  %s  /samingData "%(time, dir_path)
    log('cmd:'+cmd)

    run_cmd('rm /samingData/seafile-data/storage/ -rf')#必须删除，否则报错。   恢复后必须重启。
    if run_cmd('rm /samingData/* -rf') == -1:
        print 'rm /samingData Error!'
    if run_cmd(cmd) == -1:
        log('run_cmd Error!')
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
            write_datalib.write_datalib(username,backup_source,backup_target,'13',int(all_schedule))
            last_schedule = int(all_schedule)
            return
        #现在写入的进度没变的话，将不再写入数据库。
        if last_schedule == int(all_schedule):
            continue
        write_datalib.write_datalib(username,backup_source,backup_target,'13',int(all_schedule))
        last_schedule = int(all_schedule)

#还原samingData文件夹较慢
def write_data_soft(all_schedule,num,username):
    import time
    global last_schedule
    one_file = 0.01
    while True:
        time.sleep(3)
        all_schedule = all_schedule + one_file
        if all_schedule > num:
            all_schedule=num
            write_datalib.write_datalib(username,backup_source,backup_target,'13',int(all_schedule))
            last_schedule = int(all_schedule)
            return
        write_datalib.write_datalib(username,backup_source,backup_target,'13',int(all_schedule))
        last_schedule = int(all_schedule)

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


def last_schedule_setting():
    global last_schedule
    if 100 - last_schedule > 5:
        while(1):
            last_schedule = last_schedule + 3
            if last_schedule >= 100:
                break
            write_datalib.write_datalib(username,backup_source,backup_target ,'13',last_schedule)
            time.sleep(2)


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


#还原功能：
if __name__=='__main__':
    global last_schedule
    #初始化日志文件
    log_init()
    #获取还原点的位置。
    #read_file("/home/pi/work/conf/revert_back_path.conf")
    try:
        file = open("/home/pi/work/conf/revert_back_path.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/revert_back_path.conf Error!")
        exit()
    revert_back_path = file.read()
    file.close()

    #获取谁在使用互拷功能。
    try:
        file = open("/home/pi/work/conf/username.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    username=file.read()   #用户名
    file.close()
    
    revert_back_path = ''.join(revert_back_path.split())
    print revert_back_path

    #取出挂载位置对应的是第几个分区。
    backup_target = '/samingData'
    backup_source = revert_back_path.split('/')[5]
    print backup_source

    #先写入百分之0,让进度条跑起来。
    write_datalib.write_datalib(username,backup_source,backup_target ,'13',0)
    last_schedule = 0

    t = threading.Thread(target=write_data,args=(float(0),float(5),username))  
    t.start()
    if revert_datalib(revert_back_path) == -1:
        log("revert_datalib Error! eixt!")
        try:
            stop_thread(t)
        except:
            pass
        time.sleep(2)
        #write_datalib.write_operating_history_datalib(username,'13',-1)
        write_datalib.write_datalib(username,backup_source,backup_target ,'13',-1)
        time.sleep(9)
        write_datalib.write_datalib('-1','-1','-1','-1',-1)
        exit()
    try:
        stop_thread(t)#数据库恢复完成之后强制停止线程。  可能会有延迟，导致进度条回退，但是回退幅度不是很大。
    except:
        pass

    #恢复/samingData
    t2 = threading.Thread(target=write_data_soft,args=(float(5),float(99),username))  
    t2.start()
    if revert_data(revert_back_path) == -1:
        try:
            stop_thread(t2)
        except:
            pass
        time.sleep(2)
        log("revert_data Error! exit!")
        write_datalib.write_datalib(username,backup_source,backup_target ,'13',-1)
        #write_datalib.write_operating_history_datalib(username,'13',-1)
    else:
        try:
            stop_thread(t2)
        except:
            pass
        time.sleep(2)
        last_schedule_setting()
        write_datalib.write_datalib(username,backup_source,backup_target ,'13',100)
        #write_datalib.write_operating_history_datalib(username,'13',100) 
    
    time.sleep(9)
    write_datalib.write_datalib('-1','-1','-1','-1',-1)
    log('revert success!')
    run_cmd('sync')
    run_cmd('reboot')
    run_cmd('reboot')
    exit()
    
    #备份的数据存在，还原备份的数据。
    