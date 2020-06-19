#!/usr/bin/python
# -*- coding: UTF-8 -*-
import os
process = os.popen('ls -l')
output = process.read()
print output
process.close()