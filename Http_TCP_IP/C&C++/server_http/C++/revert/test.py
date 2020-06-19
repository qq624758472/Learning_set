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
import datetime,time
import json
last_schedule = 0
#20180626114027
def translate_time(time):
    year = time[0:4]
    mon = time[4:6]
    day = time[6:8]
    shi = time[8:10]
    fen = time[10:12]
    miao = time[12:14]
    tmp = '/increments.'+year+'-'+mon+'-'+day+'T'+shi+':'+fen+':'+miao+'+'+'08:00.dir'
    return tmp

def revert_data(revert_back_path):
    dir_time = revert_back_path.split('/')[-1]
    time = translate_time(dir_time)
    dir_path = revert_back_path.split('/')[0]+'/'+revert_back_path.split('/')[1]+'/'+revert_back_path.split('/')[2]+'/'+revert_back_path.split('/')[3]+'/'+revert_back_path.split('/')[4]+'/'+revert_back_path.split('/')[5]+'/'+revert_back_path.split('/')[6]+'/backupdata'
    cmd = 'rdiff-backup -r now '+dir_path+'/rdiff-backup-data'+time+' '+'/samingData'
    print cmd

def liu():
    global last_schedule
    last_schedule = 88
#还原功能：
if __name__=='__main__':
    # dir = " /home/pi/work/mnt/disk3/HomeCloudBackup"
    # #dir = 'hello world'
    # #dir.replace(' ','')
    # dir = ''.join(dir.split())
    # print dir

    # tmp = list(dir)
    # tmp[0] = ''
    # dir = ''.join(tmp)
    # print "dir is [%s]"%dir
    # if os.path.exists(dir) == False:
    #     print "%s not exits!"%dir
    # else:
    #     print "%s exits!"%dir
    

    last_schedule = 50
    # for filename in os.listdir(r'/home/pi/work/mnt/disk3/HomeCloudBackup'):
    #     print filename
    liu()
    # revert_data("/home/pi/work/mnt/disk3/HomeCloudBackup/20180626114027")
    #备份的数据存在，还原备份的数据。
    if 100 - last_schedule > 10:
        while(1):
            if last_schedule >= 100:
                break
            last_schedule= last_schedule + 2
            print last_schedule
            time.sleep(1)
    
