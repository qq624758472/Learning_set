#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <unistd.h>

/* #define DEBUG */

#define SERVERPORT 33923
#define SERVERQUEUELENGTH 65535
#define MAX_LINE 1024
#define SMALL_LINE 16
#define MAX_BUF_SIZE 1024
#define DEFAULT_HTTP_PORT "80"
#define SUCCESS 0
#define FAILURE -1

#define DEBUG

/* logging Macros */
#define WHERESTR  "[function %s, line %d]: "
#define WHEREARG  __FUNCTION__, __LINE__
#define __myfprintf(output, _fmt, ...) fprintf(output, WHERESTR _fmt, WHEREARG, ##__VA_ARGS__)

#define err_printf(...) __myfprintf(stderr, __VA_ARGS__)
#ifdef DEBUG
#define dbg_printf(...) __myfprintf(stdout, __VA_ARGS__)
#else
#define dbg_printf(...)
#endif

void set_signal_handlers();
int start_server_socket(int server_port);
void close_socket(int sockfd);

int write_http_response_header(int fd, char *type, int size, int code, char *code_str);
int write_http_response_data(int fd, char *buffer, int size);
int write_http_response_error(int err_code, int fd);

#endif /* _COMMON_H_ */
