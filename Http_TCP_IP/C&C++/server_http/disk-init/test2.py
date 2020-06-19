 #!/usr/bin/python
# -*- coding: UTF-8 -*-


#测试获取linux磁盘信息
  
""" import logging
import subprocess
 
def get_disk_info():
    try:
        Diskinfolist = str(subprocess.Popen(['df', '-h'], stdout=subprocess.PIPE, shell=True).communicate()).split('\\n')
    except Exception as e:
        print "error"
        return
    for i in Diskinfolist:
        if i.startswith('/dev/sd'):
            info = i.split()
            print info
            Size = float(round(float(info[1]) / 1024 , 2))
            print Size

if __name__=='__main__':
   get_disk_info()  """
   
import json
Size =1
Used =1
UsedPercent=1
Rest=1
strr=1
DiskInfoDic = {
                'DiskNumber': strr,
                'Size': Size,
                'Used': Used,
                'UsedPercent': UsedPercent,
                'Rest': Rest
                }

def write_diskinfo_conf():
    liu =[]
    # 打开一个文件
    fo = open("./diskinfo.conf", "w")

    liu.append(DiskInfoDic)
    liu.append(DiskInfoDic)
    dic={"diskinfo":liu}
    json_data=json.dumps(dic)
    fo.write(json_data)
    
    print liu
    # 关闭打开的文件
    fo.close()

def read_json_data():
    with open("./diskinfo.conf", "r")as f:
        data=f.read()
    json_data=json.loads(data)['diskinfo']
    for disk in json_data:
       #print disk["DiskNumber"]
       print disk





if __name__=='__main__':
    write_diskinfo_conf() 
    #read_json_data()