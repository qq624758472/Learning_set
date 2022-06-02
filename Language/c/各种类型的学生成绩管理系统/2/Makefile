CC = gcc
STD = -std=gnu99
FLAG = -Wall -Werror
OBJ = main.o student.o teacher.o admin.o tools.o sams.o

all:$(OBJ)
	$(CC) $(STD) $(FLAG) $(OBJ)

%.o:%.c
	$(CC) $(STD) $(FLAG) -c $<

clean:
	rm $(OBJ)
