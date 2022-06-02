#ifndef _SERVER_H_
#define _SERVER_H_

#include "ed_epoll_event.h"
#define EVENT_WAIT_TIMEOUT 1000
#define BUF_SIZE_PER_READ 1024

void handle_epoll_events();

int ed_accept_callback(struct epoll_event *event, void *data);
int ed_socket_callback(struct epoll_event *event, void *data);

#endif /* _SERVER_H_ */
