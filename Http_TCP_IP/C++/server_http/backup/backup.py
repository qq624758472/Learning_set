#!/usr/bin/env python
# -*- coding: UTF-8 -*-


#3中备份，  1.增量备份的单独文件夹。    2.按时间点备份的单独文件夹。
#------>seafile_bak
#	|------>201804251043
#	|	|------>624758472@qq.com
#	|	|	|------>共享资料库
#	|	|	|	|------>图片等
#	|	|	|------>私人资料库
#	|	|	|	|------>音乐等
#	|	|------670077157@qq.com
#	|	|	|------>共享资料库
#	|	|	|	|------>图片等
#	|	|	|------>私人资料库
#	|	|	|	|------>音乐等
#	|------>increase_seafile_bak   自己的想法，暂时没做。



import os
import datetime
import write_datalib
import time,datetime
import threading  
import inspect  
import ctypes  
import simplejson as json
dir_time_all=None

cmd_umount = '/home/pi/work/disk-init/umount-all.sh'

backup_source = '/samingData'
backup_target = 'null'

last_schedule = 0


def last_schedule_setting():
    global last_schedule
    if 100 - last_schedule > 5:
        while(1):
            last_schedule = last_schedule + 3
            if last_schedule >= 100:
                break
            write_datalib.write_datalib(username,backup_source,backup_target ,'12',last_schedule)
            time.sleep(2)



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

    #seahub得单独处理。
    def combination_seahub(self):
        back_posion = self.back_posion + "/" + self.data_name + ".sql"
        cmd = "mysqldump -uroot -proot --ignore-table=seahub_db.saming_singleuser %s > %s"%(self.data_name,back_posion)
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
    if ccnet.combination_seahub() == False:
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

#dir 为HomeCloud文件夹。
def back_data_all(dir): 
    cmd = "rdiff-backup -b " + "/samingData " + dir
    if run_cmd(cmd) == -1:
        log("[run cmd] is Error!")
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


def if_mon(mon):
    if mon == 'Jan':
        return '01'
    elif mon == 'Feb':
        return '02'
    elif mon == 'Mar':
        return '03'
    elif mon == 'Apr':
        return '04'
    elif mon == 'May':
        return '05'
    elif mon == 'Jun':
        return '06'
    elif mon == 'Jul':
        return '07'
    elif mon == 'Aug':
        return '08'
    elif mon == 'Sep':
        return '09'
    elif mon == 'Oct':
        return '10'
    elif mon == 'Nov':
        return '11'
    elif mon == 'Dec':
        return '12'
    else:
        return '-1'

def get_backup_list(dir):
    cmd = "rdiff-backup -l " + dir
    r = os.popen(cmd)       #执行该命令
    info = r.readlines()    #读取命令行的输出到一个list
    r.close()
    for line in info:       #按行遍历
        line = line.strip('\r\n')
        if line == None:
            return 1
        try :
            if line.split()[0] == 'Current':
                return line.split()[6]+if_mon(line.split()[3])+line.split()[4]+line.split()[5].replace(':','')
        except:
            pass

#同步备份点，将多余备份的数据库文件删除
# def sync_backpoint(dir):
#     cmd = "rdiff-backup -l " + dir
#     r = os.popen(cmd)       #执行该命令
#     info = r.readlines()    #读取命令行的输出到一个list
#     for line in info:       #按行遍历
#         line = line.strip('\r\n')
#         if line == None:
#             return 1
#         for filename in os.listdir(dir):
#             print filename


def write_data(one_file,all_schedule,num,username):
    global last_schedule   #修改全局变量需要global声明
    import time
    if one_file == None:
        one_file = 0.1
    while True:
        if all_schedule > num:
            all_schedule = num
            write_datalib.write_datalib(username,backup_source,backup_target,'12',int(all_schedule))
            last_schedule = int(all_schedule)
            return
        time.sleep(3)
        all_schedule = all_schedule + one_file
        #现在写入的进度没变的话，将不再写入数据库。
        if last_schedule == int(all_schedule):
            continue
        write_datalib.write_datalib(username,backup_source,backup_target,'12',int(all_schedule))
        last_schedule = int(all_schedule)


def py_get_time_dir():
    with open('/home/pi/work/conf/username.conf', 'r') as f:
        return f.read()
     
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
            run_cmd(cmd_umount)
            log('not found dev_name!')
            write_datalib.write_datalib('-1','-1','-1','-1',-1)
            time.sleep(1)
            write_datalib.write_datalib('-1','-1','-1','-1',-1)
            os._exit(0) #结束整个进程
        time.sleep(2)




#备份功能：
#1.备份中，如果其中任何一个出错，就会退出并删除备份的文件夹，防止用户以为备份成功，然后恢复错误的备份导致系统崩溃。
#2.
if __name__=='__main__':
    global last_schedule
    #初始化日志文件
    cmd = "echo Start backup   > /home/pi/work/backup/backup.log"
    os.system(cmd)

    #获取用户挂载的位置。
    mount_posion=get_mount_posion()
    if mount_posion == -1:
        log("mount_posion is NULL")
        exit()
    #获取谁在使用互拷功能。
    try:
        file = open("/home/pi/work/conf/username.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    username=file.read()   #用户名
    file.close()

    #取出挂载位置对应的是第几个分区。
    backup_source = '/samingData'
    backup_target = read_json_data(mount_posion)
    if backup_target == -1:
        log('read_json_data return None!')
        print('read_json_data return None!')
        exit()

    #判断挂载的位置是否存在HomeCloudBackup文件夹
    dir = mount_posion+"/HomeCloudBackup"
    dir_data = dir + '/backupdata'
    log(dir)
    isExists = os.path.exists(dir)
    if not isExists:
        #一般都是False,说明没有这个文件夹。
        log("[%s]not exits ,start create"%dir)
        cmd = "mkdir "+dir
        log(cmd)
        if run_cmd(cmd)== -1:
            log("[%s] create Error!" % dir)
            exit()

    saming_path='/samingData'

    write_datalib.write_datalib(username,backup_source,backup_target ,'12',1)
    last_schedule = 1
    #备份/samingData
    one_file = calculate_schedule(saming_path)  #一个文件所占的百分比
    t = threading.Thread(target=write_data,args=(one_file,float(1),float(95),username))  
    t.start()

    #起一个线程，用来检测用户是否拔掉硬盘
    if_exit_disk = threading.Thread(target=if_disk_be_pulled_pthread, args=(backup_target, t))
    if_exit_disk.start()

    if back_data_all(dir_data) == -1:
        try: 
            stop_thread(t)
        except:
            pass
        #write_datalib.write_operating_history_datalib(username,'12',-1)
        write_datalib.write_datalib('-1','-1','-1','-1',-1)
        log('back samingData Error!')
        exit()
    #备份完成之后强制停止线程。
    try: 
        stop_thread(t)
    except:
        pass

    #获取上个函数备份的文件日期
    backup_time = get_backup_list(dir_data)
    dir_time = dir + "/" + backup_time
    with open("/home/pi/work/conf/backup_time.conf", "w")as f:
        f.write(dir_time)
    isExists = os.path.exists(dir_time)
    if not isExists:
        #一般都是False,说明没有这个文件夹。
        log("[%s]not exits ,start create"%dir_time)
        cmd = "mkdir "+dir_time
        log(cmd)
        if run_cmd(cmd)== -1:
            log("[%s] create Error!" % dir_time)
            exit()

    #备份数据库文件，占用5/100
    back_datalib_all(dir_time)

    #备份完成之后强制停止线程。
    try: 
        stop_thread(if_exit_disk)
    except:
        pass

    run_cmd('sync')
    run_cmd(cmd_umount)

    #在进度条变到100之前，让进度条平滑走完。
    last_schedule_setting()

    write_datalib.write_datalib(username,backup_source,backup_target,'12',100)
    time.sleep(2)
    write_datalib.write_datalib(username,backup_source,backup_target,'12',100)
    
    
    time.sleep(9)
    write_datalib.write_datalib('-1','-1','-1','-1',-1)
    log('backup success!')
    os._exit(0) #结束整个进程


