SOURCE=1.asm
CC=nasm
LD=ld
TARGET=main

all:1.asm
	$(CC) -f elf $(SOURCE)
	$(LD) -m elf_i386 1.o -o $(TARGET)

clean:
	rm *.o
	rm $(TARGET)
