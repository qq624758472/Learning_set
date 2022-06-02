
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "common.h"
#include "ed_server.h"
#include "ed_epoll_event.h"
#include "ed_http.h"
#include "thread_pool.h"

ed_epoll_t ed_epoll;

struct threadpool *pool = NULL;

int main(int argc, char *argv[])
{
    int server_port;
    int server_sockfd;

    if (argc < 2 || (server_port = atoi(argv[1])) <= 0)
    {
        err_printf("Usage: %s port\n", argv[0]);
        exit(1);
    }

    set_signal_handlers();

    //初始化内存池
    pool = threadpool_init(10, 20);

    /* initialize epoll object */
    memset(&ed_epoll, 0, sizeof(ed_epoll_t));
    if (ed_epoll_init(&ed_epoll, MAX_ED_EVENTS))
    {
        err_printf("ed_epoll_init: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
        exit(1);
    }

    /* Start HTTP Server  and Listen for incoming requests */
    server_sockfd = start_server_socket(server_port);
    if (server_sockfd < 0)
    {
        err_printf("Could not start server on port %d\n", server_port);
        exit(1);
    }

    /* Set the socket to non-blocking */
    if (setnonblock(server_sockfd) < 0)
    {
        close_socket(server_sockfd);
        err_printf("failed to set server socket %d to non-blocking\n", server_sockfd);
        exit(1);
    }

    /* accept requests 将服务器fd放入epoll集合中*/
    ed_epoll_add(&ed_epoll, server_sockfd, ed_accept_callback, NULL);
    handle_epoll_events();

    /* close server socket */
    close_socket(server_sockfd);

    return 0;
}

/* start handling epoll events */
void handle_epoll_events()
{
    while (1)
    {
        ed_epoll_dispatch_events(&ed_epoll, EVENT_WAIT_TIMEOUT);

        /* do any cleanup */
    }
}

/* callback for events on listening server socket */
int ed_accept_callback(struct epoll_event *event, void *data)
{
    int new_fd;
    struct sockaddr_in client_addr;
    socklen_t len;
    char client_ip4[INET_ADDRSTRLEN];

    dbg_printf("Got new request\n");

    len = sizeof(struct sockaddr_in);

    /* there could be more than one incoming connections
    可能有多个传入连接 */
    while (1)
    {
        new_fd = -1;
        new_fd = accept(event->data.fd, (struct sockaddr *)&client_addr, &len);

        if (new_fd == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                /* we have processed all requests */
                break;
            }
            else
            {
                err_printf("accept failed for fd %d, code: %d, Error: %s\n", event->data.fd, errno, strerror(errno));
                break;
            }
        }

        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip4, INET_ADDRSTRLEN);
        // dbg_printf("got request. new socket %d from: %s\n", new_fd, client_ip4);
        dbg_printf("got request from: %s:%d, assign socket is:%d\n", client_ip4, ntohs(client_addr.sin_port), new_fd);
        /* Set client socket to non-blocking */
        if (setnonblock(new_fd) < 0)
        {
            close_socket(new_fd);
            err_printf("failed to set client socket %d to non-blocking\n", new_fd);
            break;
        }

        ed_epoll_add(&ed_epoll, new_fd, ed_socket_callback, NULL);
    }

    return SUCCESS;
}

/* handle events on http request sockets */
int ed_socket_callback(struct epoll_event *event, void *data)
{
    // char buf[BUF_SIZE_PER_READ];
    int read_len, fd, n;

    dbg_printf("received event for socket %d\n", event->data.fd);
    // memset(buf, 0, BUF_SIZE_PER_READ);
    fd = event->data.fd;

    if (event->events & EPOLLERR)
    {
        dbg_printf("Event:EPOLLERR %d on socket %d\n", event->events, fd);
    }
    else if (event->events & EPOLLHUP)
    {
        dbg_printf("Event:EPOLLHUP %d on socket %d\n", event->events, fd);
        ed_epoll_del(&ed_epoll, fd);
        close_socket(fd);
    }
    else if (event->events & EPOLLRDHUP)
    {
        dbg_printf("Event:EPOLLRDHUP %d on socket %d\n", event->events, fd);
        ed_epoll_del(&ed_epoll, fd);
        close_socket(fd);
    }
    else if (event->events & EPOLLOUT)
    {
        if (data)
        {
            dbg_printf("we have something to write on socket %d\n", fd);
            write_http_response(&ed_epoll, fd, data);
            if (ed_epoll.ed_clients[fd].http_req.status == STATUS_REQUEST_FINISH)
            {
                ed_epoll_del(&ed_epoll, fd);
                close_socket(fd);
            }
        }
        else
        {
            ed_epoll_set(&ed_epoll, fd, EPOLLIN, NULL);
        }
    }
    else if (event->events & EPOLLIN)
    {
        struct ed_client *client = &ed_epoll.ed_clients[fd];
        int max_read;

        dbg_printf("data available to read on %d\n", fd);

        read_len = 0;
        n = 0;
        // do
        {
            max_read = ((client->buf_len + BUF_SIZE_PER_READ) <= REQUEST_SIZE) ? BUF_SIZE_PER_READ : (REQUEST_SIZE - client->buf_len);
            n = read(fd, (client->buffer + client->buf_len), max_read);
            if (n > 0)
            {
                read_len += n;
                client->buf_len += n;
            }
        } // while(n == BUF_SIZE_PER_READ);

        dbg_printf("%d bytes(n = %d) read on socket %d\n", read_len, n, fd);

        if (n == -1)
        {
            err_printf("read on socket %d failed. Code: %d, Error %s\n", fd, errno, strerror(errno));
            if (errno == EWOULDBLOCK)
            {
                err_printf("EWOULDBLOCK\n");
                if (read_len == 0)
                    err_printf("no data to read then why I got read event\n");
            }
        }
        else
        {
            /* start http state machine */
            dbg_printf("Echo: %s\n", client->buffer);
            if (process_http_request(client->buffer, &client->http_req) == SUCCESS)
            {
                if (client->http_req.page != NULL)
                {
                    /* page requested */
                    dbg_printf("page requested: %s on socket %d\n", client->http_req.page, fd);
                    ed_epoll_set(&ed_epoll, fd, EPOLLOUT, client->http_req.page);
                }
            }
            else if (client->buf_len > 0 && client->buffer[client->buf_len - 1] == '\n')
            {
                err_printf("illegal page request on socket %d\n", fd);
                ed_epoll_del(&ed_epoll, fd);
                close_socket(fd);
            }
        }
    }
    else
    {
        dbg_printf("Event %d on socket %d\n", event->events, fd);
    }

    return SUCCESS;
}
