#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：server.py
 
import socket               # 导入 socket 模块
#import pymysql              #
import login
import json

#从获取的命令到执行
class command():    
    def __init__(self,data):
        self.data = data
        self.cmd = data['cmd']
    def run(self):
        if self.cmd == 'login':  #登录
            return login.clientLogin(data['username'], data['passwd'])
        elif self.cmd == 'regist':  #新用户注册
            return login.clientRegist(data['username'], data['passwd'])
        elif self.cmd == 'null': #丢包，或缓存挤压
            print ('丢包')
            return False
        else:
            return False

def format_json(data):
    data = data.decode('utf-8')
    try:
        son = json.loads(data)
    except:
        son ="{\"cmd\":\"null\"}"
        son = json.loads(son)
    return son

if __name__=='__main__':
    s = socket.socket()         # 创建 socket 对象
    host = socket.gethostname() # 获取本地主机名
    port = 12345                # 设置端口
    s.bind(("0.0.0.0", port))        # 绑定端口

    s.listen(5)                 # 等待客户端连接
    buffer = []
    while True:
        c,addr = s.accept()     # 建立客户端连接
        print (addr)
        #while True:
        buffer = []             #存储发来的数据，用户名   密码
        d = c.recv(1024)
        if d:
            buffer.append(d)
        else:
            print ('recv Error!')
        data = b''.join(buffer)
        print ("recv:%s"%data)
        data = format_json(data)        # 将字符串格式化成 json
        t = command(data)
        if t.run() == True:
            c.send('Success'.encode("utf-8"))
        else:
            c.send('Error'.encode("utf-8"))
        c.close()                # 关闭连接
        print ("断开该客户端连接")
