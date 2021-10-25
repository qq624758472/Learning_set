#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：server.py
 
import socket               # 导入 socket 模块


#从获取的命令到执行
class command():
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    

if __name__=='__main__':
    s = socket.socket()         # 创建 socket 对象
    host = socket.gethostname() # 获取本地主机名
    port = 12345                # 设置端口
    s.bind(("0.0.0.0", port))        # 绑定端口

    s.listen(5)                 # 等待客户端连接
    while True:
        c,addr = s.accept()     # 建立客户端连接
        print ('连接地址：%s',addr)
        c.send('loginSuccess'.encode("utf-8"))
        c.close()                # 关闭连接