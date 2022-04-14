CC=gcc
prom = main
deps = 
obj = main.o server.o
obj_cli = client.o
arg += -pthread -g

$(prom): $(obj)
	$(CC) -o $(prom) $(obj) $(arg)

client:$(obj_cli)
	$(CC) -o client $(obj_cli)

$(obj_cli):%.o:%.c
	$(CC) -c $(arg) $< -o $@

#%.o:%.c 所有的.o目标都依赖于与它同名的.c文件
#当然还有deps中列出的头文件
#$<  第一个依赖文件    $@目标文件   $^所有的依赖文件
.c.o:
	$(CC) -Wall -o $@ -c $< $(arg)
 
clean:
	rm -rf $(prom) *.o client