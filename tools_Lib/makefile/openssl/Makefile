include ../common.mk
include openssl.mk


target=3.o


#下面两种方式都可以生成.o文件。

#静态规则
#通过通配符生成所有.o文件。
$(target):%.o:%.c
	$(cc) -c $< -o $@
	cp $@ ../


#自动生成依赖
#通过自动生成依赖，生成3.o: 3.c ../common.h
#然后隐式规则运行cc -c -o 3.o 3.c进行编译
#因为是隐式规则生成的，所以不能进行拷贝，所以还是使用上边的
# include $(target:.o=.d)
# %.d: %.c
# 	$(cc) -MM $< > $@


.PHONY : clean
clean:
	-rm $(target:.o=.d)
	-rm $(target)