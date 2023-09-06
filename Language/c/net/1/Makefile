
ARG += -g -lpthread -O0 -static

src_tcp += tcp_pack.c 
src_udp += udp_pack.c

CC = arm-linux-gnueabihf-gcc

obj_tcp += $(src_tcp:.c=.o)
obj_udp += $(src_udp:.c=.o)

.PHONY:clean all tcp_send udp_send icmp_send

all: icmp_send tcp_send udp_send

tcp_send:$(obj_tcp)
	$(CC) $(ARG) $(obj_tcp) -o tcp_send

udp_send:$(obj_udp)
	$(CC) $(ARG) $(obj_udp) -o udp_send


icmp_send:icmp_pack.o main.o icmp_pack.h
	$(CC) -o icmp_send icmp_pack.o main.o -lm $(ARG)
icmp_pack.o:icmp_pack.c
	$(CC) -c icmp_pack.c -o icmp_pack.o -lm $(ARG)
main.o:main.c
	$(CC) -c main.c -o main.o -lm $(ARG)

$(obj_tcp):%.o:%.c
	$(CC) $(ARG) -o $@ -c $<
$(obj_udp):%.o:%.c
	$(CC) $(ARG) -o $@ -c $<



clean:
	-rm -rf *.o
	-rm tcp_send udp_send icmp_send