CC=g++
prom = main
deps = common.h my_string.h sort.h stream.h template.h p_class.h
obj = main.o my_string.o sort.o stream.o template.o p_class.o

$(prom): $(obj)
	$(CC) -o $(prom) $(obj)

#%.o:%.c 所有的.o目标都依赖于与它同名的.c文件
#当然还有deps中列出的头文件
#$<  第一个依赖文件    $@目标文件   $^所有的依赖文件
%.o:%.c $(deps)
	$(CC) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)