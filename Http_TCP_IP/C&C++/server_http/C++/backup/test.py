#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import os
import datetime
import write_datalib
import time,datetime
import threading  
import inspect  
import ctypes  
def run_cmd():
    pass

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
    for line in info:       #按行遍历
        line = line.strip('\r\n')
        if line == None:
            return 1
        try :
            #print line.split()[0]
            if line.split()[0] == 'Current':
                return line.split()[6]+if_mon(line.split()[3])+line.split()[4]+line.split()[5].replace(':','')
        except:
            pass


#同步备份点，将多余备份的数据库文件删除
def sync_backpoint(dir):
    cmd = "rdiff-backup -l " + dir
    r = os.popen(cmd)       #执行该命令
    info = r.readlines()    #读取命令行的输出到一个list
    for line in info:       #按行遍历
        line = line.strip('\r\n')
        if line == None:
            return 1
        try :
            if line.split()[0] == 'Current':
                time = line.split()[6]+if_mon(line.split()[3])+line.split()[4]+line.split()[5].replace(':','')
                print '1-->'+line.split()[6]+if_mon(line.split()[3])+line.split()[4]+line.split()[5].replace(':','')
            else:
                time = line.split()[0].split('.')[1].split('+')[0].replace('-','').replace('T','').replace(':','')
                print '2-->'+line.split()[0].split('.')[1].split('+')[0].replace('-','').replace('T','').replace(':','')
        except:
            pass
        dirs = os.listdir(dir)
        for name in dirs:
            path = dir+'/'+name
            #print path
            if os.path.isdir(path): 
                print path
                if name == time:

if __name__=='__main__':
    dir ="/data/source"
    sync_backpoint(dir)
