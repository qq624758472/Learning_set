#加入gcov编译选项,通过宏PRJRELEASE=gcov控制
#ifeq ("$(PRJRELEASE)","gcov")
CFLAGS+= -fprofile-arcs -ftest-coverage
#endif

CC=gcc

.PHONE: all

all: main

main: *.c
#   编译出汇编和gcno文件
	@echo ${CFLAGS}
	@${CC} ${CFLAGS} -S -o main.s main.c 
	@${CC} ${CFLAGS} -o main main.c 

.PHONE: clean html

html: main
	#./main
	gcov *.c
	lcov -c -d . -o  main.info
	genhtml -o dir_html main.info

clean:
	@-rm main *.gcno *.gcda *.c.gcov *.s *.info
	@-rm dir_html -rf