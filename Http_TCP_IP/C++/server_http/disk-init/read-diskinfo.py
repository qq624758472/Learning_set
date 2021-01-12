 #!/usr/bin/python
# -*- coding: UTF-8 -*-


   
import json


def read_json_data():
    with open("./diskinfo.conf", "r")as f:
        data=f.read()
    json_data=json.loads(data)['DiskInfo']
    for disk in json_data:
       print disk["mount_posion"]
       print disk["seize_part_capacity"]





if __name__=='__main__':
    read_json_data()