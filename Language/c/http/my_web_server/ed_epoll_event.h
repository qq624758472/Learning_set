/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-05-06 09:58:27
 * @LastEditors: ruog__
 * @LastEditTime: 2022-05-21 15:28:21
 */
#ifndef _EPOLL_EVENT_H_
#define _EPOLL_EVENT_H_

#include <sys/epoll.h>

#define MAX_ED_EVENTS 65535
#define REQUEST_SIZE 4096
#define READ_CHUNK_SIZE 1024

#define STATUS_REQUEST_START  0
#define STATUS_REQUEST_FINISH 1
#define STATUS_FILE_FOUND     2
#define STATUS_HEADER_WRITTEN 3
#define STATUS_DATA_WRITING   4
#define STATUS_CGI_FILE_FOUND 5
#define STATUS_CGI_PROC_RUNNING 6
#define STATUS_CGI_RESPONSE_WRITING 7

typedef int (*event_callback_func_t)(struct epoll_event* event, void* data);

/* keeps track of currently serving request */
typedef struct parsed_request {
  char page[MAX_LINE];
  int status;
  FILE *fp;
  int response_size;
  char cgi_command[MAX_LINE];
  char cgi_query_str[MAX_LINE];
} parsed_request_t;

/* associate a handler to fd */
typedef struct ed_client {
  event_callback_func_t callback;
  void *data;
  int fd;
  char buffer[REQUEST_SIZE];
  int buf_len;
  int buf_parsed_len;
  parsed_request_t http_req;
} ed_client_t;

/* keep tracks of all epoll descriptors */
typedef struct ed_epoll {
  int maxfds;
  int epoll_fd;
  int listener_fd;
  ed_client_t *ed_clients;  
  struct epoll_event *epoll_events;
  int fds_used;
} ed_epoll_t;


typedef struct _ed_threadpool_task
{
    ed_client_t *ed_client;
    struct epoll_event *epoll_event;
} ed_threadpool_task;

int setnonblock(int fd);
int ed_epoll_init(struct ed_epoll *, int maxfds);
int ed_epoll_add(struct ed_epoll *, int fd, event_callback_func_t callback, void *data);
int ed_epoll_set(struct ed_epoll *this, int fd, int eventcode, void* data);
int ed_epoll_del(struct ed_epoll *, int fd);

int ed_epoll_dispatch_events(struct ed_epoll *epoll_obj, int timeout);

#endif /* _EPOLL_EVENT_H_ */
