# Makefile2.6
obj-m += myIrq.o        # 产生hellomod 模块的目标文件
CURRENT_PATH := $(shell pwd)   #模块所在的当前路径
LINUX_KERNEL := $(shell uname -r)    #Linux内核源代码的当前版本
LINUX_KERNEL_PATH := /usr/src/linux-headers-$(LINUX_KERNEL) #Linux内核源代码的绝对路径
#LINUX_KERNEL_PATH := /home/hao/my_work/kernel_source/linux-2.6.0/
all:
	#编译模块了
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	#清理
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean