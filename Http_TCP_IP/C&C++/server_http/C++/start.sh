#!/bin/bash

#初始化数据库
/home/pi/work/init_mysql.sh
/home/pi/work/create_history_mysql.sh

#初始化互拷模块锁
rm /home/pi/work/conf/ifcontinue.conf -rf

#判断/home/pi/work是否存在，存在时跳过，否则，copy /saming/work  到/home/pi/work
if [ -d "/home/pi/work/" ];then
	echo "/home/pi/work/ aleady exists" > /home/pi/work/start.log
else
	cp /saming/work/ /home/pi/ -rf
    cd /home/pi/work/data_control_server/
    chmod -R 777 /home/pi/work
    ./make.sh
fi

cd  /home/pi/work/data_control_server/
if [ -f "/home/pi/work/data_control_server/server" ];then
	rm /home/pi/work/data_control_server/server
    ./make.sh
    /home/pi/work/data_control_server/server
else	
    chmod -R 777 /home/pi/work
    ./make.sh
    ./server
fi
