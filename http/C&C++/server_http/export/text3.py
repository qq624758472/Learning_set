#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
import datetime
import time
import threading  
import inspect  
import ctypes  
import simplejson as json

def format_data_lib_capacity(data_lib_capacity):
	data = int( data_lib_capacity.split()[0] )
	danwei = data_lib_capacity.split()[1]
	if danwei == 'bytes':
		if data == 0:
			return 0
		else:
			data = data / 1024 /1024
			return data
	elif danwei == 'GB':
		return data
	elif danwei == 'MB':
		if data == 0:
			return 0
		else:
			data = data / 1024
			return data
	
if __name__=='__main__':
	data_lib_capacity='1029 MB'
	capacity = format_data_lib_capacity(data_lib_capacity)
	print "%d GB"%capacity