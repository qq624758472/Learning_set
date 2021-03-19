CC=gcc
prom = main
deps = 
obj = main.o
ARG += -g

CSRCS = $(wildcard ./*.c)
CPPSRCS = $(wildcard ./*.cpp)

$(prom): $(CSRCS:.c=.o) $(CPPSRCS:.cpp=.o)
	$(CC)  -g -o $(prom) $(obj) 

#c和cpp混合编译的makefile示例
#$<  第一个依赖文件    $@目标文件   $^所有的依赖文件
$(CPPSRCS) : %.o: %.cpp
	ifeq ($(@:.o=.cpp),$(wildcard *.cpp))
	$(CC) -g -c $< -o $@ 
	endif

$(CSRCS) : %.o: %.c
	$(CC) -g -c $< -o $@ 

clean:
	rm -rf $(obj) $(prom)