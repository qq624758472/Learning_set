#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import threading  
import time  
import inspect  
import ctypes  
    
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
    
def write_data(one_file,all_schedule,num):  
    while 2:  
            print(one_file)
            print(all_schedule)
            print(num)
            print(111111111111)  
            print(222222222222)  
            print(333333333333)  
            print(444444444444)  
            print(555555555555)  
            print(666666666666)  
            time.sleep(1)
    
    
if __name__ == "__main__":  
    one_file=00000
    t = threading.Thread(target=write_data,args=(one_file,10,55))  
    t.start()  
    #time.sleep(10)
    #stop_thread(t)  
    print("stoped")  
    exit()