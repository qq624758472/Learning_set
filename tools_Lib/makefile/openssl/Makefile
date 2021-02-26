include ../common.mk
include openssl.mk


target=3.o
#target_base=lib

target_dir=../lib/

.PHONY : clean lib

#下面两种方式都可以生成.o文件。

#静态规则
#通过通配符生成所有.o文件。
$(target):%.o:%.c
	$(cc) -c $< -o $@
	cp $@ ../


lib:$(target_share) $(target_static)
	@cp $(target_static) $(target_dir)/$(target_base)$(target_static)
	@cp $(target_share) $(target_dir)/$(target_base)$(target_share)

$(target_share):%.so:%.c
	$(cc) -fPIC -shared $< -o $@
$(target_static):%.a:%.c
	$(cc) -c $< -o $(@:.a=.o)
	$(ar) -rc $@ -o $(<:.c=.o)

#ar -rc libxxx.a xxx.c

#自动生成依赖
#通过自动生成依赖，生成3.o: 3.c ../common.h
#然后隐式规则运行cc -c -o 3.o 3.c进行编译
#因为是隐式规则生成的，所以不能进行拷贝，所以还是使用上边的
# include $(target:.o=.d)
# %.d: %.c
# 	$(cc) -MM $< > $@



clean:
	-rm $(target:.o=.d)
	-rm $(target) $(target_static) $(target_share)