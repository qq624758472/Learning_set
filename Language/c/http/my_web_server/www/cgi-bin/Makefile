CC = gcc
CFLAGS = -O2 -Wall -I ..

all: generate_text

generate_text: generate_text.c
	$(CC) $(CFLAGS) -o generate_text generate_text.c

clean:
	rm -f generate_text *~ *.exe
