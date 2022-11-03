


all:
	gcc -o main main.c


#预处理
eee:
#-C 表示不删除注释信息
	gcc -E -C main.c -o main.i
	gcc -E main.c -o main.I 
build:
	gcc -S main.i -o main.S
sss:
	gcc -c main.S -o main.o
lll:
	gcc main.o -o main