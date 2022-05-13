CC=gcc
prom = main
deps = common.h
src := main.c test_c.c
obj = $(src:.c=.o)
CFLAGS += -g

$(prom): $(obj)
	$(CC) -o $(prom) $(obj) $(CFLAGS)

#%.o:%.c 所有的.o目标都依赖于与它同名的.c文件
#当然还有deps中列出的头文件
#$<  第一个依赖文件    $@目标文件   $^所有的依赖文件
%.o:%.cpp $(deps)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(obj) $(prom)