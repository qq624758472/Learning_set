#!/bin/bash
echo liu
username=$(cat /home/pi/work/conf/username.conf)
mount_posion=$(cat /home/pi/work/conf/mount_posion.conf)
share_or_person=$(cat /home/pi/work/conf/share_or_person.conf)

echo share_or_person is ->$share_or_person

username=$(cat /home/pi/work/conf/username.conf)
mount_posion=$(cat /home/pi/work/conf/mount_posion.conf)
share_or_person=$(cat /home/pi/work/conf/share_or_person.conf)
echo share_or_person is  $share_or_person
if [ $share_or_person == "1" ];then   	#私密
	echo [/home/pi/work/conf/share_or_person.conf] is 1
elif [ $share_or_person == "2" ]; then 	#共享
	echo [/home/pi/work/conf/share_or_person.conf] is 2 
else		   								#私密+共享
	echo /home/pi/work/conf/share_or_person.conf is 3
fi