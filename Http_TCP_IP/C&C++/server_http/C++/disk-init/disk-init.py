#!/usr/bin/python
# -*- coding: UTF-8 -*-



import logging
import os
import subprocess
import json
import urllib2
import urllib
mount_disk = 1


class disk_info():
    #分区总数
    part_num = 0                #分区总数
    part_num_access = 0         #挂载成功的分区数

    list_dev = []    #/dev/sdb3   设备名
    list_mount = []  #挂载的地方
    list_part_capacity = []    #分区容量
    list_seize_part_capacity = []    #分区已用容量


df = disk_info()


#打开或者关闭log
all_log = 1
#全局设置的log
mod = "disk-init.log"
# 创建一个logger
logger = logging.getLogger('disk-init')
logger.setLevel(logging.DEBUG)

# 创建一个handler，用于写入日志文件
path = "/home/pi/work/disk-init/%s" % mod
#print path #创建之前先删除
os.system("rm %s" % path)
fh = logging.FileHandler(path)
fh.setLevel(logging.DEBUG)  

# 再创建一个handler，用于输出到控制台
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)

# 定义handler的输出格式
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
ch.setFormatter(formatter)

# 给logger添加handler
logger.addHandler(fh)
logger.addHandler(ch)



def run_cmd(cmd):
    if_return = os.system(cmd)
    if if_return != 0:
        return -1
    return 1

def print_err(run_cmd):
    logger.info("Run :[%s] Error!"%(run_cmd))
    print "Run :[%s] Error!"%(run_cmd)




def get_list_seize_part_capacity(part,if_mount):
    if if_mount == -1:
        seize_part_capacity = -1
        df.list_seize_part_capacity.append(seize_part_capacity)
        return 
    
    p = subprocess.Popen(['df', '-h'], stdout=subprocess.PIPE, shell=True)
    try:
        Diskinfolist = str(p.communicate()).split('\\n')
    except:
        print_err("get_list_seize_part_capacity()")
        return
    for i in Diskinfolist:
        if i.startswith(part):  
            info = i.split()
            size = float(round(float(info[2]) / 1024 , 2))
    seize_part_capacity = size
    df.list_seize_part_capacity.append(seize_part_capacity)
    p.wait()  #不等待,shell会变僵尸
    return 


def get_list_part_capacity(part,if_mount):   #挂载一个分区，算出一个分区的容量，如果容量未获取到，对应的列表位置也得放-1
    if if_mount == -1:
        capacity = -1
        df.list_part_capacity.append(capacity)
        return 
        
    p = subprocess.Popen(['df', '-h'], stdout=subprocess.PIPE, shell=True)
    try:
        Diskinfolist = str(p.communicate()).split('\\n')
    except:
        print_err("get_list_part_capacity()")
        return
    for i in Diskinfolist:
        if i.startswith(part):
            info = i.split()
            size = float(round(float(info[1]) / 1024 , 2))
    capacity = size
    df.list_part_capacity.append(capacity)
    p.wait()  #不等待,shell会变僵尸
    return 

def get_list_dev(part):  
    df.list_dev.append(part)
    return 

def get_list_mount(mount_tmp):  
    df.list_mount.append(mount_tmp)
    return 

def disk_part_info(disk_name):
    global mount_disk #想要给全局变量赋值，就必须声明
    #global df
    
    cmd = "ls "
    cmd = cmd[:3] + disk_name + '*'
    #print cmd
    logger.info("cmd is [%s]"%cmd)

    process = os.popen(cmd)
    output = process.read()
    process.close()
    #print output
    logger.info("output is [%s]"%output)

    #已经查到了所有/dev/下的所有设备
    for num in range(1,9):
        try:
            part = output.split("\n",9)[num]
        except:
            pass
        else:
            print part  #存在设备就挂载。
            if part == "": #去掉最后一个空字符串
                continue
                
            #找到一个分区，分区数+1
            df.part_num = df.part_num + 1
            #linux下挂载windowsfat32等其他格式时，中文会乱码问题。
            cmd = "mount -o  iocharset=utf8 %s /home/pi/work/mnt/disk%d"% (part,mount_disk)

            mount_tmp = cmd.split(part)[1]

            #print "----------------------------->%s" % mount_tmp
            mount_disk = mount_disk + 1
            #print cmd
            if run_cmd(cmd) == -1:
                print_err(cmd)
                mount_tmp = -1  #未挂载成功的话，列表中赋值-1
                #continue
            else:   #挂载成功给挂载成功的+1
                df.part_num_access = df.part_num_access + 1

            #挂载成功的话将挂载的信息赋值到列表中      
            get_list_dev(part)  
            get_list_mount(mount_tmp) 
            get_list_part_capacity(part,mount_tmp)  #获取挂载的硬盘总容量和已用容量。
            get_list_seize_part_capacity(part,mount_tmp)
            try:
                logger.info( "df.part_num-----------------------> [%d]" %(df.part_num))
                logger.info( "df.part_num_access----------------> [%d]" %(df.part_num_access))
                logger.info( "df.list_dev[%d]-------------------> [%s]" %(mount_disk-2, df.list_dev[mount_disk-2]))
                logger.info( "df.list_mount[%d]-----------------> [%s]" %(mount_disk-2, df.list_mount[mount_disk-2]))        
                logger.info( "df.list_part_capacity[%d]---------> [%s]" %(mount_disk-2, df.list_part_capacity[mount_disk-2]))
                logger.info( "df.list_seize_part_capacity[%d]---> [%s]" %(mount_disk-2, df.list_seize_part_capacity[mount_disk-2]))
            except:
                logger.info('logger.info Error')
    return 1


def get_data_lib_for_token(url,token):
    # replace with your token
    url = url + "api2/repos/"

    req = urllib2.Request(url)
    req.add_header('Authorization', 'Token ' + token)
    response = urllib2.urlopen(req)
    the_page = response.read()

    #print the_page
    return the_page  


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

#获取资料库信息
def get_data_lib(url,username,passwd):
    token = get_token(url,username,passwd)
    the_page = get_data_lib_for_token(url,token)
    return the_page

def get_data_lib_info():
    url='http://127.0.0.1:8000/'
    try:
        file = open("/home/pi/work/conf/passwd.conf",'r')
    except IOError:
        logger.info("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    passwd = file.read()   #用户密码

    try:
        file = open("/home/pi/work/conf/username.conf",'r')
    except IOError:
        logger.info("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    username=file.read()   #用户名
    print "passwd [%s]\n"%passwd
    print "username [%s]\n"%username

    #获取资料库信息.   需要私有资料库ID 和 共享资料库ID
    size_formatted = -1
    data_name = -1
    list_data_lib_info = []#资料库名，对应的ID
    data_lib = get_data_lib(url,username,passwd)
    json_data_lib=json.loads(data_lib)
    for disk in json_data_lib:
        size_formatted = disk["size_formatted"]
        data_name = disk["name"]
        data_lib_info = {
                'size_formatted': size_formatted,
                'data_name': data_name,
                }
        list_data_lib_info.append(data_lib_info)
        # print disk
        
    return list_data_lib_info


def test():
    #=====开始获取资料库信息。
    list_data_lib_info = get_data_lib_info()
    print list_data_lib_info


def write_diskinfo_conf():
    disk_info = []
    # 打开一个文件
    fo = open("/home/pi/work/disk-init/diskinfo.conf", "w") #覆盖的方式打开。
    if fo==0:
        return -1
    part_num = df.part_num
    part_num_access = df.part_num_access
    """ dev_name = -1 
    mount_posion = -1
    part_capacity = -1
    seize_part_capacity = -1 """
    j = 0
    if_mount_error = 0

    for i in df.list_dev:
        dev_name = i
        mount_posion = df.list_mount[j]
        part_capacity = df.list_part_capacity[j]
        seize_part_capacity = df.list_seize_part_capacity[j]

        DiskInfoDic = {
                'part_num': part_num,
                'part_num_access': part_num_access,
                'dev_name': dev_name,
                'mount_posion': mount_posion,
                'part_capacity': part_capacity,
                'seize_part_capacity': seize_part_capacity,
                }
        if DiskInfoDic["mount_posion"]== -1:  #跳过未挂载成功的磁盘。
            j = j + 1
            if_mount_error = 1
            break
        disk_info.append(DiskInfoDic)
        j = j + 1
    #=====获取硬盘信息结束。

    

    if if_mount_error == 1:
        fo.write("501")
    else:
        #=====开始获取资料库信息。
        list_data_lib_info = get_data_lib_info()

        dic={"DiskInfo":disk_info,"DataLibInfo":list_data_lib_info}
        json_data=json.dumps(dic)
        fo.write(json_data)
    # 关闭打开的文件
    fo.close()


if __name__=='__main__':
    #test()
    cmd = "ls /dev/sd* |awk '{print $1}'"
    process = os.popen(cmd)
    output = process.read()
    process.close()
    #print output

    #global df   #硬盘信息

    disk_number = 0
    disk_num = ['/dev/sda', '/dev/sdb',  '/dev/sdc', '/dev/sdd', '/dev/sde', '/dev/sdf', '/dev/sdg']
    for disk_name in disk_num:        
        #print disk_name
        if output.find(disk_name) == -1 :
            #print 'not found ', disk_name
            pass
        else :
            disk_number = disk_number + 1
            #找到硬盘时，列出该硬盘分区。
            if disk_part_info(disk_name) == -1:
                #exit()
                pass
    
    if disk_number == 1:
        logger.info("disk number not enough, disk num is [%d]" % disk_number )
    
    print ("disknumber is %d， part_num = [%d]" % (disk_number,df.part_num))

    if write_diskinfo_conf() == 1:  #执行成功删除日志。
        print "write_diskinfo_conf Access!"
        # cmd = "sudo rm /home/pi/work/disk-init/disk-init.log"
        # os.popen(cmd)

    # logger.info("Disk init access!")

#硬盘数     硬盘n：分区n       硬盘占用率       硬盘大小，  已占用多少


















""" 

os.system(cmd):

该方法在调用完shell脚本后，返回一个16位的二进制数，低位为杀死所调用脚本的信号号码，高位为脚本的退出状态码，即脚本中“exit 1”的代码执行后，os.system函数返回值的高位数则是1，如果低位数是0的情况下，则函数的返回值是0×100,换算为10进制得到256。

如果我们需要获得os.system的正确返回值，那使用位移运算可以还原返回值：  """


""" os.popen(cmd):

这种调用方式是通过管道的方式来实现，函数返回一个file-like的对象，里面的内容是脚本输出的内容（可简单理解为echo输出的内容）。 """


















