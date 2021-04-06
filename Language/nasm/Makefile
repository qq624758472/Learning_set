SOURCE=2
CC=nasm
LD=ld
TARGET=main

all:$(SOURCE).asm
	$(CC) -f bin $(SOURCE).asm -o main.bin
	#$(LD) -m elf_i386 $(SOURCE).o -o $(TARGET)
	#$(LD) -m elf_x86_64 $(SOURCE).o -o $(TARGET)
	rm *.o
clean:
	rm $(TARGET)
