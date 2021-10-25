#!/bin/bash

#3中备份，  1.增量备份的单独文件夹。    2.按时间点备份的单独文件夹。
#------>seafile_bak
#	|------>201804251043
#	|	|------>624758472@qq.com
#	|	|	|------>共享资料库
#	|	|	|	|------>图片等
#	|	|	|------>私人资料库
#	|	|	|	|------>音乐等
#	|	|------670077157@qq.com
#	|	|	|------>共享资料库
#	|	|	|	|------>图片等
#	|	|	|------>私人资料库
#	|	|	|	|------>音乐等
#	|------>increase_seafile_bak   自己的想法，暂时没做。

#echo "Start fuse!"   > /home/pi/work/export/export.log

#先开始seaf-fuse服务。
if [ -d "/samingFuse/" ];then
	echo "/samingFuse aleady exists" >> /home/pi/work/export/export.log
else
	mkdir /samingFuse/
fi


cd /saming/seafile-server-6.2.2/
if [ $? -eq 0 ];then
	echo "cmd [cd /saming/seafile-server-6.2.2/] Access!" >> /home/pi/work/export/export.log
else
	echo "cmd [cd /saming/seafile-server-6.2.2/] Error!"  >> /home/pi/work/export/export.log
fi

#判断是否运行
ps -x |grep seaf-fuse | grep -v grep
if [ $? -eq 0 ]; then 
	echo 正在运行 >> /home/pi/work/export/export.log
else
	echo 没有运行 >> /home/pi/work/export/export.log
	./seaf-fuse.sh start /samingFuse/
fi

#开始拷贝数据到指定的分区中
if [ -f "/home/pi/work/conf/username.conf" ];then
	echo [/home/pi/work/conf/username.conf] is exist >> /home/pi/work/export/export.log
else	
	echo [/home/pi/work/conf/username.conf] is not exist >> /home/pi/work/export/export.log
	exit   #测试过，可以退出
fi

if [ -f "/home/pi/work/conf/mount_posion.conf" ];then
	echo [/home/pi/work/conf/mount_posion.conf] is exist >> /home/pi/work/export/export.log
else	
	echo [/home/pi/work/conf/mount_posion.conf] is not exist >> /home/pi/work/export/export.log
	exit   #测试过，可以退出
fi

if [ -f "/home/pi/work/conf/share_or_person.conf" ];then
	echo [/home/pi/work/conf/share_or_person.conf.conf] is exist >> /home/pi/work/export/export.log
else	
	echo [/home/pi/work/conf/share_or_person.conf.conf] is not exist >> /home/pi/work/export/export.log
	exit   #测试过，可以退出
fi

username=$(cat /home/pi/work/conf/username.conf)
mount_posion=$(cat /home/pi/work/conf/mount_posion.conf)
share_or_person=$(cat /home/pi/work/conf/share_or_person.conf)

datename=$(date +%Y%m%d%H%M%S)   
mkdir $mount_posion"/"$datename   
mkdir $mount_posion"/"$datename"/"$username


echo $mount_posion"/"$datename
echo share_or_person is  $share_or_person
echo username is  $username	
if [ "$share_or_person" -eq "1" ];then   	#私密
	mkdir $mount_posion"/"$datename"/"$username"/"private
	echo [/home/pi/work/conf/share_or_person.conf] is 1 >> /home/pi/work/export/export.log
	cp /samingFuse/$username/*/* $mount_posion"/"$datename"/"$username"/"private -rf		 #导出自己本身文件夹。
	if [ $? != 0 ];then
		echo cmd [ cp /samingFuse/$username/*/* $mount_posion"/"$datename"/"$username"/"private -rf ]  Error! >> /home/pi/work/export/export.log
	fi
elif [ "$share_or_person" -eq "2" ]; then 	#共享
	mkdir $mount_posion"/"$datename"/"$username"/"share
	echo [/home/pi/work/conf/share_or_person.conf] is 2 >> /home/pi/work/export/export.log
	cp /samingFuse/share@share.com/*/* $mount_posion"/"$datename"/"$username"/"share -rf    #导出共享文件夹。
	if [ $? != 0 ];then
		echo cmd [ cp /samingFuse/share@share.com/*/* $mount_posion"/"$datename -rf ]  Error! >> /home/pi/work/export/export.log
	fi
else		   								#私密+共享
	mkdir $mount_posion"/"$datename"/"$username"/"private
	mkdir $mount_posion"/"$datename"/"$username"/"share
	echo /home/pi/work/conf/share_or_person.conf is 3 >> /home/pi/work/export/export.log
	cp /samingFuse/$username/*/*  $mount_posion"/"$datename"/"$username"/"private -rf		 #导出自己本身文件夹。
	if [ $? != 0 ];then
		echo cmd [ cp /samingFuse/$username/*/* $mount_posion"/"$datename"/"$username"/"private -rf ]  Error! >> /home/pi/work/export/export.log
	fi
	cp /samingFuse/share@share.com/*/* $mount_posion"/"$datename"/"$username"/"share -rf    #导出共享文件夹。
	if [ $? != 0 ];then
		echo cmd [ cp /samingFuse/share@share.com/*/* $mount_posion"/"$datename"/"$username"/"share -rf ]  Error! >> /home/pi/work/export/export.log
	fi
fi





































