CC=g++
prom = main
deps = common.h my_string.h sort.h stream.h template.h p_class.h DuoTai.h
SRC += $(wildcard *.cpp)
OBJ += $(SRC:.cpp=.o)

$(prom): $(OBJ)
	$(CC) -o $(prom) $(OBJ)

#%.o:%.c 所有的.o目标都依赖于与它同名的.c文件
#当然还有deps中列出的头文件
#$<  第一个依赖文件    $@目标文件   $^所有的依赖文件
%.o:%.cpp $(deps)
	$(CC) -c $< -o $@

#自动生成依赖文件
include $(SRC:.cpp=.d)
%.d: %.cpp
	$(CC) -MM $< > $@

clean:
	rm -rf $(OBJ) $(prom) $(SRC:.cpp=.d)