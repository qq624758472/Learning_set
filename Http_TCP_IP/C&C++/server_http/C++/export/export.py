#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import urllib2
import urllib
import requests
import datetime
import write_datalib
import time
import threading  
import inspect  
import ctypes  
import simplejson as json
import urlparse
import httplib, mimetypes
from requests_toolbelt import  *



cmd_umount = '/home/pi/work/disk-init/umount-all.sh'

backup_source = 'null'
backup_target = 'null'
last_schedule = 0

def export_data(username):  #导出数据
    return

def get_token(url,username,password):
    '''
    acquire token
    '''  
    url = '%s/api2/auth-token/'%url
    values = {'username': username,
          'password': password}
    data = urllib.urlencode(values)
    req = urllib2.Request(url, data)
    response = urllib2.urlopen(req)
    the_page = response.read()
    token = json.loads(the_page)['token']
    return token


def run_cmd(cmd):
    if_return = os.system(cmd)
    if if_return != 0:
        return -1
    return 1

def log(revert_log):
    cmd = "echo %s   >> /home/pi/work/export/export.log" % revert_log
    os.system(cmd)

def log_init():
    cmd = "echo Start export!   > /home/pi/work/export/export.log"
    os.system(cmd)
    return 1

def last_schedule_setting():
    global last_schedule
    if 100 - last_schedule > 5:
        while(1):
            last_schedule = last_schedule + 3
            if last_schedule >= 100:
                break
            write_datalib.write_datalib(username,backup_source,backup_target,'11',last_schedule)
            time.sleep(2)


#导出数据较慢。
def write_data(all_schedule,num,username,capacity):
    global last_schedule
    import time
    #判断总量为几个G . 每1G 加 600秒。
    if capacity == 0:
        one_file = 0.32
    elif capacity == -1:
        one_file = 0.01
    else:
        #one_file = (100.0 / (capacity * 600.0)) * 4
        one_file = 2.0 / (3.0 * capacity)
    

    #print 'one_file is ------------%f'%one_file
    while True:
        time.sleep(4)
        all_schedule = all_schedule + one_file
        if all_schedule > num:
            all_schedule=num
            write_datalib.write_datalib(username,backup_source,backup_target,'11',int(all_schedule))
            last_schedule = int(all_schedule)
            return
        #现在写入的进度没变的话，将不再写入数据库。
        if last_schedule == int(all_schedule):
            continue
        write_datalib.write_datalib(username,backup_source,backup_target,'11',int(all_schedule))
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


def read_json_data(mount_posion):
    with open("/home/pi/work/disk-init/diskinfo.conf", "r")as f:
        data=f.read()
    json_data=json.loads(data)['DiskInfo']
    if json_data == None:
        print 'json_data is NULL!'
        return -1
    for disk in json_data:
    #    print disk["mount_posion"]
    #    print mount_posion
       dir = ''.join(disk["mount_posion"].split())
       if dir == mount_posion:
           return ''.join(disk["dev_name"].split())
    return -1



#判断要导出的硬盘是否被拔掉
def if_disk_be_pulled(dev_name):
    cmd = "fdisk -l"
    r = os.popen(cmd)       #执行该命令
    info = r.readlines()    #读取命令行的输出到一个list
    r.close()
    #print info
    for line in info:       #按行遍历
        try:
            #print line.split()[0]
            if line.split()[0] == dev_name:
                #print '111111111111111111111111'
                return 1
        except:
            pass
    return -1


#线程函数，判断是否拔掉硬盘， 拔掉的化，结束进程，并进行初始化
def if_disk_be_pulled_pthread(dev_name, t):
    global cmd_umount
    import time
    while True:
        if if_disk_be_pulled(dev_name) == -1:
            print 'Disk already be pulled!'
            log('Disk already be pulled!')
            try:
                stop_thread(t)
            except:
                try:
                    stop_thread(t)
                except:
                    print 'stop thread Error!'
                    log('stop thread Error!')
            time.sleep(3)
            write_datalib.write_operating_history_datalib(username,'11',-1,export_time,share_or_person,dev_name)
            run_cmd('sync')
            run_cmd(cmd_umount)
            log('not found dev_name!')
            write_datalib.write_datalib('-1','-1','-1','-1',-1)
            os._exit(0) #结束整个进程
        time.sleep(2)

#format capacity . unit :GB
def format_data_lib_capacity(data_lib_capacity):
    try:
        data = float( data_lib_capacity.split()[0] )
        danwei = data_lib_capacity.split()[1]
        #danwei = ''.join(data_lib_capacity.split()[1])
    except:
        data = 0
        danwei = 'bytes'

    data = int(data)
    if danwei == 'bytes':
        if data == 0:
            return 0
        else:
            data = (data / 1024 /1024) + 1
            return data
    elif danwei == 'GB':
        return data
    elif danwei == 'MB':
        if data == 0:
            return 0
        else:
            data = (data / 1024) + 1
            return data
    else:
        return -1


def get_data_lib_for_token(url,token):
    # replace with your token
    url = url + "api2/repos/"

    req = urllib2.Request(url)
    req.add_header('Authorization', 'Token ' + token)
    response = urllib2.urlopen(req)
    the_page = response.read()

    #print the_page
    return the_page  


#获取资料库信息
def get_data_lib(url,username,passwd):
    token = get_token(url,username,passwd)
    the_page = get_data_lib_for_token(url,token)
    #print the_page
    return the_page

def get_datalib_info(username, passwd, share_or_person):
    #获取资料库信息.   需要私有资料库大小 和 共享资料库大小
    url='http://127.0.0.1:8000/'
    list_data_lib_info = []#资料库名，对应的数据大小
    data_lib = get_data_lib(url,username,passwd)

    json_data_lib=json.loads(data_lib)
    for disk in json_data_lib:
        data_lib_name = disk["name"]
        data_lib_capacity = disk["size_formatted"]
        data_lib_info = {
                    'data_lib_name': data_lib_name,
                    'data_lib_capacity': data_lib_capacity,
                    }
        if data_lib_info['data_lib_name'] == u"私人资料库" or data_lib_info['data_lib_name'] == u"共享资料库":
            list_data_lib_info.append(data_lib_info)

    #format two data lib capacity
    for lib_name in list_data_lib_info:
        if lib_name['data_lib_name'] == u'私人资料库':
            data_lib_capacity = lib_name['data_lib_capacity']
            person_capacity = format_data_lib_capacity(data_lib_capacity)
        elif lib_name['data_lib_name'] == u'共享资料库':
            data_lib_capacity = lib_name['data_lib_capacity']
            share_capacity = format_data_lib_capacity(data_lib_capacity)
        else:
            pass
    print 'share_capacity is %d \n, person_capacity is %d \n'%(share_capacity, person_capacity)
    if share_or_person == '1':  # 1.私人资料库
        log_info = "person_capacity is %d"%person_capacity
        print log_info
        log(log_info)
        return person_capacity
    elif share_or_person == '2':  #2.共享资料库
        log_info = "share_capacity is %d"%share_capacity
        print log_info
        log(log_info)
        return share_capacity
    elif share_or_person == '3':  #3.全部导出
        log_info = "capacity is %d"%(share_capacity + person_capacity)
        print log_info
        log(log_info)
        return (share_capacity + person_capacity)
    else:
        data_lib_capacity = -1
        log("Parameter Error!")
        return -1


if __name__=='__main__':
    global last_schedule
    global cmd_umount
    log_init()
    #获取谁在使用互拷功能。
    try:
        file = open("/home/pi/work/conf/username.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/username.conf Error!")
        exit()
    username=file.read()   #用户名
    file.close()

    try:
        file = open("/home/pi/work/conf/passwd.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    passwd = file.read()   #用户密码

    try:
        file = open("/home/pi/work/conf/share_or_person.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/share_or_person.conf Error!")
        exit()
    share_or_person=file.read() 
    file.close()

    try:
        file = open("/home/pi/work/conf/mount_posion.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/mount_posion.conf Error!")
        exit()
    mount_posion=file.read() 
    file.close()

    print username,passwd, share_or_person
    #get data lib capacity
    try:
        capacity = get_datalib_info(username, passwd, share_or_person)
    except:
       capacity = -1
       print'get_datalib_info() Error!'

    print '----capacity is %d-------'%capacity
    #取出挂载位置对应的是第几个分区。
    backup_source = share_or_person
    dev_name = backup_target = read_json_data(mount_posion)
    if backup_target == -1:
        log('read_json_data return None!')
        exit()

    #等于-1说明挂载的盘被拔掉。
    if if_disk_be_pulled(dev_name) == -1:
        write_datalib.write_datalib(username,backup_source,backup_target,'11',-1)
        write_datalib.write_operating_history_datalib(username,'11',-1,export_time,share_or_person,dev_name) 
        time.sleep(9)
        write_datalib.write_datalib('-1','-1','-1','-1',-1)
        run_cmd(cmd_umount)
        log('not found dev_name!')
        exit()


    export_time  = datetime.datetime.now().strftime('%Y.%m.%d %H:%M:%S')

    #导出之前写写进度条
    write_datalib.write_datalib(username,backup_source,backup_target ,'11',0)
    last_schedule = 0

    t = threading.Thread(target=write_data,args=(float(0),float(99),username,capacity))  
    t.start()

    print "------------------------------------------"
    #起一个线程，用来检测用户是否拔掉硬盘
    if_exit_disk = threading.Thread(target=if_disk_be_pulled_pthread, args=(dev_name, t))
    if_exit_disk.start()

    cmd = "sudo /home/pi/work/export/seafile-bak.sh"
    if run_cmd(cmd) == -1:
        log('run ./seafile-bak.sh error!')  
        try: 
            stop_thread(t)
        except:
            pass
        time.sleep(2)
        write_datalib.write_datalib(username,backup_source,backup_target,'11',-1)
        write_datalib.write_operating_history_datalib(username,'11',-1,export_time,share_or_person,dev_name) 
        time.sleep(9)
    else:
        try:
            stop_thread(t)
        except:
            pass
        time.sleep(2)
        last_schedule_setting()
        write_datalib.write_datalib(username,backup_source,backup_target,'11',100)
        write_datalib.write_operating_history_datalib(username,'11',100,export_time,share_or_person,dev_name)
        time.sleep(9)

    try:
        stop_thread(if_exit_disk)
    except:
        pass
    write_datalib.write_datalib('-1','-1','-1','-1',-1)
    run_cmd('sync')
    run_cmd(cmd_umount)
    log('export success!')
    os._exit(0) #结束整个进程