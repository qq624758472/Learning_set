#ifndef _ED_HTTP_H_
#define _ED_HTTP_H_

#define WWW_PATH "www"
#define FASTCGI_SERV_ADDR "settlers.lab.ece.cmu.local"
#define FASTCGI_SERV_PORT "12345"

#include "ed_epoll_event.h"

int process_http_request(char *header, parsed_request_t *http_req);
int write_http_response(struct ed_epoll *epoll_obj, int fd, void *data);
char *get_file_type(char *file_name);
int write_http_cgi_response(struct ed_epoll *epoll_obj, char *file_path, int fd);
FILE *run_cgi_file(char *file_path, char *query_str);
int write_http_fastcgi_response(struct ed_epoll *epoll_obj, char *file_path, int fd);

#endif /* _ED_HTTP_H_ */
