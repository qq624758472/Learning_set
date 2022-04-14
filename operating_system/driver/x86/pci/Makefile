# Makefile2.6
obj-m += main.o        # 产生hellomod 模块的目标文件
CURRENT_PATH := $(shell pwd)   #模块所在的当前路径
LINUX_KERNEL := $(shell uname -r)    #Linux内核源代码的当前版本
x86_LINUX_KERNEL_PATH := /usr/src/kernels/3.10.0-1127.el7.x86_64/ 				#Linux内核源代码的绝对路径  x86版本
arm_LINUX_KERNEL_PATH := /home/hao/work_my/kernel_source/linux-4.19.90			#Linux内核源代码的绝对路径  arm版本编译后的。 


x86:helloApp_x86
	make -C $(x86_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
	
arm:helloApp_arm
	make -C $(arm_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules

clean:
	-make -C $(x86_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
	-rm helloApp_x86

clean_arm:
	-make -C $(arm_LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean


helloApp_x86:$(obj-m:.o=.c)

helloApp_arm:helloApp.c
	arm-linux-gnueabi-gcc helloApp.c -o $@ -static




