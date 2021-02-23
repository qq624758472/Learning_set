include common.mk
objects=1.o 2.o 3.o ./call1/4.o
object12=1.o 2.o 2.c
src=1.c 2.c

SRC_DIR := src
SUBDIR := 

#include的用法
#include中只有一个变量：arg=-L/usr/local/lib/  -lssl -lcrypto
include ./openssl/openssl.mk

.PHONY : clean openssl ENV if order　tell


#main编译必须先编译3.c以后才能开始编译。
main:openssl $(objects)
	$(cc)  $(objects) -o main $(arg)


#make的嵌套调用
openssl:
	$(MAKE) -C openssl


#没啥作用就是为了显示环境变量
ENV:
	echo $(PATH)

# #显式规则
# 1.o:1.c
# 	$(cc) -c 1.c -o 1.o $(arg)
# 2.o:2.c
# 	$(cc) -c 2.c -o 2.o $(arg)


#隐式规则, 甚至可以不写。
# 1.o:1.c
# 2.o:2.c


# #静态规则
# $(object12) : %.o: %.c
# 	$(cc) -c $< -o $@  $(arg)
# #静态规则中$(filter)的用法
# $(filter %.o,$(object12)) : %.o: %.c
# 	$(cc) -c $< -o $@  $(arg)



# #自动生成依赖文件
# include $(src:.c=.d)
# %.d: %.c
# 	$(cc) -MM $< > $@


#if函数---设置默认值
#如果变量SRC_DIR的值不为空,则将SRC_DIR指定的目录作为SUBDIR子目录;否则将/home/src作为子目录
SUBDIR += $(if $(SRC_DIR),$(SRC_DIR),/home/src)
if:
	@echo $(SUBDIR)


#重新顺序化参数　函数call
reverse = $(2) $(1) $(3)
foo = $(call reverse,q,w,e)
order:
	@echo $(foo)

#origin　函数的作用是告诉你变量是哪里来的，其出生状况如何，他并不改变变量。
tell:
	@echo $(origin @)


#1. .PHONY意思表示clean是一个“伪目标”
#2. clean的规则不要放在文件的开头，不然，这就会变成make的默认目标
#3. 在rm命令前面加了一个小减号的意思就是，也许某些文件出现问题，但不要管，继续做后面的事.  不然若出错后边命令将不执行。
clean:
	-rm main
	-rm $(src:.c=.d)
	-rm $(objects)
	cd openssl;$(MAKE) clean
	cd call1;$(MAKE) clean