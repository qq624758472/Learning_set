#!/usr/bin/python
# -*- coding: UTF-8 -*-
import logging

'''
mod:哪个模块调用的。
'''
def log_init(mod): 
    # 创建一个logger
    logger = logging.getLogger('mylogger')
    logger.setLevel(logging.DEBUG)

    # 创建一个handler，用于写入日志文件
    path = "../%s" % mod
    print path
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





