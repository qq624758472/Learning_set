include common.mk
obj_base=2.o
objects=1.o 2.o 3.o call1/4.o
object12=1.o 2.o 2.c
object_share_lib=1.so openssl/3.so
object_static_lib=1.a openssl/3.a


src=1.c 2.c
target_dir=./lib/

SRC_DIR:= openssl
SUBDIR:= 

#使用库方式调用函数类型 1.isStatic 2.isShare 3.isMix 4.other
IS_LIB_TYPE=other  

#include的用法
#include中只有一个变量：arg=-L/usr/local/lib/  -lssl -lcrypto
include ./openssl/openssl.mk

.PHONY : clean openssl ENV if order　tell main  static_lib share_lib openssl_lib　isMix　isShare　isStatic call1


#命令包的用法。
define make_subdir
	@for subdir in $(SUBDIR) ; \
	do \
		( cd $$subdir && make $1) \
	done;
endef

#main编译必须先编译3.c以后才能开始编译。　　第一个目标为makefile默认目标。
main: openssl $(object12) call1 $(obj_base)
	#直接编译成程序。
	$(cc) $(objects) -o main $(arg) $(arg_base)
isMix:openssl_lib  $(object_share_lib) $(object_static_lib) $(obj_base)
	#0.使用混合方式
	$(cc) $(obj_base) -o main  $(arg_base)  $(arg)
isShare:openssl_lib  $(object_share_lib) $(obj_base)
	#1.使用动态库
	$(cc) $(obj_base) -o main  $(arg) $(arg_base)
isStatic:openssl_lib $(object_static_lib) $(obj_base)
	#2.使用静态库
	$(cc) $(obj_base) -o main  $(arg) $(arg_base)



# $(object_share_lib):%.so:%.c
# 	$(cc) -fPIC -shared $< -o $@
# 	@cp $@ $(target_dir)/$(target_base)$@
# $(object_static_lib):%.a:%.c
# 	$(cc) -c $< -o $(@:.a=.o) $(arg_base)
# 	$(ar) -rs $@ $<
# 	@cp $@ $(target_dir)/$(target_base)$@


$(object_share_lib):%.so:%.c
	$(cc) -fPIC -shared $< -o $@
	@cp $@ $(target_dir)/$(target_base)$@
$(object_static_lib):%.a:%.c
	$(cc) -c $< -o $@ $(arg_base)
	@cp $@ $(target_dir)/$(target_base)$@



#条件编译参数
ifeq ($(IS_LIB_TYPE),isMix)
#0.混合链接
arg += -L$(target_dir) -l1
arg += openssl/3.a
else ifeq ($(IS_LIB_TYPE),isShare)
#1.使用动态链接
arg += -L$(target_dir) -l1 
arg += -L$(target_dir) -l3
else ifeq ($(IS_LIB_TYPE),isStatic)
#2.使用静态链接
arg += $(object_static_lib)
else
#不进行操作
endif



#make的嵌套调用
openssl:$(object_share_lib:.so=.c)
	$(MAKE) -C openssl
openssl_lib:$(object_share_lib:.so=.c)
	$(call make_subdir ,lib)
call1:
	$(MAKE) -C call1

#没啥作用就是为了显示环境变量
ENV:
	echo $(PATH)

# #显式规则
# 1.o:1.c
# 	$(cc) -c 1.c -o 1.o $(arg_base)
# 2.o:2.c
# 	$(cc) -c 2.c -o 2.o $(arg_base)


#隐式规则, 甚至可以不写。
# 1.o:1.c
# 2.o:2.c


# #静态模式
# $(object12) : %.o: %.c
# 	$(cc) -c $< -o $@  $(arg_base)
# #静态规则中$(filter)的用法：过滤
# $(filter %.o,$(object12)) : %.o: %.c
# 	$(cc) -c $< -o $@  $(arg_base)



# #自动生成依赖文件
# include $(src:.c=.d)
# %.d: %.c
# 	$(cc) -MM $< > $@


#if函数---设置默认值
#如果变量SRC_DIR的值不为空,则将SRC_DIR指定的目录作为SUBDIR子目录;否则将/home/src作为子目录
SUBDIR += $(if $(SRC_DIR),$(SRC_DIR),./openssl)
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
	-rm $(object_static_lib)
	-rm $(object_share_lib)
	cd openssl;$(MAKE) clean
	cd call1;$(MAKE) clean
	-rm lib/*