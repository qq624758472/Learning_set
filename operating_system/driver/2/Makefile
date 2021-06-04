# Makefile2.6
obj-m += helloworldDriver.o        # 产生hellomod 模块的目标文件
CURRENT_PATH := $(shell pwd)   #模块所在的当前路径
LINUX_KERNEL := $(shell uname -r)    #Linux内核源代码的当前版本
x86_LINUX_KERNEL_PATH := /usr/src/linux-headers-$(LINUX_KERNEL) 				#Linux内核源代码的绝对路径  x86版本
arm_LINUX_KERNEL_PATH := /home/hao/work_my/kernel_source/linux-4.19.90			#Linux内核源代码的绝对路径  arm版本编译后的。 


x86:helloApp_x86
	make -C $(x86_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
	
arm:helloApp_arm
	make -C $(arm_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules

clean:
	#清理
	-make -C $(arm_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
	-make -C $(x86_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
	-rm helloApp_arm
	-rm helloApp_x86


helloApp_x86:helloApp.c

helloApp_arm:helloApp.c
	arm-linux-gnueabi-gcc helloApp.c -o $@ -static

#make命令需要借助（arm_LINUX_KERNEL_PATH）目录下的Makefile文件， 如果编译arm平台的驱动，则需要修改提前编译好的arm平台的内核模块。
#参考这篇文章http://blog.chinaunix.net/uid-23080322-id-118420.html




