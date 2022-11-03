BASE := arm-linux-#aarch64-linux-gnu-   #arm-linux-
CC := $(BASE)gcc   
AS :=  $(BASE)as
LD :=  $(BASE)ld
OJ :=  $(BASE)objcopy
LDCONFIG += -static
LDCONFIG += -g


all:#这两个没有区别，都可以编译成目标文件
#	$(AS) asm_add.S -o add.o
	$(CC) -c asm_add.S -o add.o

	$(CC) -E -C c_add.c -o c_add.i
	$(CC) -S c_add.i -o c_add.S
	$(CC) -c c_add.S -o c_add.o

	$(CC) -E -C main.c -o main.i
	$(CC) -S main.i -o main.S
	$(CC) -c main.S -o main.o

#实验成功
#	$(CC) $(LDCONFIG) main.o add.o c_add.o -o main  #可以指定静态链接

#编译成功,裸板bin文件未实验
#	$(LD) -nostadlib -o bootstrap.elf -Tlink.lds main.o add.o c_add.o
#	$(OJ) -O binary bootstrap.elf bootstrap.bin


#通过链接脚本编译，可以编译通过，显示：./main: Invalid argument
	$(LD) -nostadlib -o main -Tlink.lds main.o add.o

#通过LD链接，可以编译通过，显示:qemu: uncaught target signal 11 (Segmentation fault) - core dumped
#	$(LD) main.o add.o -o main


#单独编译asm_add.S
add:
# 将.S ARM汇编源码 编译成 .o 目标文件
	$(AS) -o add.o asm_test.S
# 将.o 目标文件 链接成 可执行文件  hello
	$(LD) add.o -o add


clean:
	-rm *.i *.o main add main.S *.elf *.bin