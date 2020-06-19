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
import json


list_restory_point = []
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




def list_path_dir(dir):
    #先列出该文件夹下的所有文件夹。
    for filename in os.listdir(dir):
        print filename
        RestorePoint = {
            'mount_posion':dir,
            'restory_point_name':filename,
        }
        list_restory_point.append(RestorePoint)
    


#获取到挂载上的所有磁盘信息。
def get_mount_posion():
    list_mount_posion = [] 
    with open("/home/pi/work/disk-init/diskinfo.conf", "r")as f:
        data=f.read()
    json_data=json.loads(data)['DiskInfo']
    for disk in json_data:
        print "mount_posion is [%s]"%disk["mount_posion"]
        mount_posion = disk["mount_posion"]
        #判断备份的数据是否存在HomeCloudBackup文件夹   不存在的话，跳过
        dir = mount_posion+"/HomeCloudBackup"
        dir = ''.join(dir.split())
        print "dir is [%s]"%dir
        if os.path.exists(dir) == False:
            print "%s not exits!"%dir
            log("%s not exits!"%dir)
        else:
            print "%s exits!"%dir
            log("%s exits!"%dir)
            list_mount_posion.append(dir)
    print "list_mount_posion return !"
    return list_mount_posion

def log_init():
    cmd = "echo Start revert!   > /home/pi/work/revert/revert.log"
    os.system(cmd)
    return 1

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
    dir_backdata = dir  + '/backupdata'
    #先检查备份文件是否完整。
    cmd_check = 'rdiff-backup --check-destination-dir '+ dir_backdata
    log('cmd_check:'+cmd_check)
    run_cmd(cmd_check)

    cmd = "rdiff-backup -l " + dir_backdata
    r = os.popen(cmd)       #执行该命令
    info = r.readlines()    #读取命令行的输出到一个list
    r.close()
    for line in info:       #按行遍历
        line = line.strip('\r\n')
        if line == None:
            return list_restory_point
        try :
            if line.split()[0] == 'Current':
                dir_time = line.split()[6]+if_mon(line.split()[3])+line.split()[4]+line.split()[5].replace(':','')
                print line.split()[6]+if_mon(line.split()[3])+line.split()[4]+line.split()[5].replace(':','')
            else:
                dir_time = line.split()[0].split('.')[1].split('+')[0].replace('-','').replace('T','').replace(':','')
                print line.split()[0].split('.')[1].split('+')[0].replace('-','').replace('T','').replace(':','')
            RestorePoint = {
            'mount_posion':dir,
            'restory_point_name':dir_time,
            }
            list_restory_point.append(RestorePoint)
        except:
            pass


#还原功能：
if __name__=='__main__':
    #初始化日志文件
    log_init()
    fo = open("/home/pi/work/conf/RestorePoint.conf", "w") #覆盖的方式打开。
    if fo==0:
        print('open [/home/pi/work/conf/RestorePoint.conf] Error!')
        log('open [/home/pi/work/conf/RestorePoint.conf] Error!')
        exit()
    #获取用户挂载的位置。
    mount_posion=get_mount_posion()
    if mount_posion == -1:
        print("mount_posion is NULL")
        log("mount_posion is NULL")
        exit()
    print len(mount_posion)
    for index in range(len(mount_posion)):
        log("mount_posion[%s]=[%s]"%(index,mount_posion[index]))
        #list_path_dir(mount_posion[index])
        get_backup_list(mount_posion[index])
        
    
    dic = {'RestorePoint':list_restory_point}
    json_data=json.dumps(dic)
    fo.write(json_data)
    fo.close()

    
    #备份的数据存在，还原备份的数据。
    