EXE = server
CC = gcc
CFLAGS = -g -Wall -rdynamic
LDFLAGS = -lpthread -ldl
DEPS = common.h ed_epoll_event.h ed_http.h ed_server.h thread_pool.h
WEBOBJ = common.o ed_epoll_event.o ed_http.o ed_server.o thread_pool.o


server: $(WEBOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o $(EXE)
