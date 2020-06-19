#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import time,datetime
# try:
#     file = open("/home/pi/work/conf/import_data.conf",'r')
# except IOError:
#     print("open /home/pi/work/conf/import_data.conf Error!")
#     exit()
# share_or_person = file.read() #导入到共享资料库或者个人资料库
# if share_or_person == '1':
#     print "access"
# else:
#     print "error"
time  = datetime.datetime.now().strftime('%Y.%m.%d %H:%M:%S')
print time
