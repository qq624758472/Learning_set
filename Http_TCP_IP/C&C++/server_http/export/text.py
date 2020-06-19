#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os

if __name__=='__main__':
    capacity = 5
    if capacity == 0:
        one_file = 0.32
    else:
        one_file = (100.0 / (capacity * 600.0)) * 4

    print one_file