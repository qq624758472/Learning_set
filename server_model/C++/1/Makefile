.SUFFIXES:.cpp .o
 
CC=g++
 
SRCS=server.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=server
 
start: $(OBJS)
	$(CC) -g -o $(EXEC) $(OBJS)
	@echo "-----------------------------OK-----------------------"
 
.c.o:
	$(CC) -g -Wall -o $@ -c $<
 
clean:
	rm -rf $(EXEC) $(OBJS)
