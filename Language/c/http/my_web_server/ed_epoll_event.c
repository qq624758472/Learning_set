#include <assert.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "common.h"
#include "ed_epoll_event.h"
#include "thread_pool.h"

extern struct threadpool *pool;

/**
 * Set a socket to non-blocking mode.
 */
int setnonblock(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
        return -1;

    return 0;
}

/**
 * @brief Initialize application epoll object which keeps track
 *        of all events and associated clients
 */
int ed_epoll_init(struct ed_epoll *epoll_obj, int maxfds)
{
    int i;

    dbg_printf("ed_epoll_init\n");

    epoll_obj->maxfds = maxfds;
    epoll_obj->fds_used = 0;

    epoll_obj->ed_clients = (ed_client_t *)malloc(sizeof(ed_client_t) * maxfds);
    if (!epoll_obj->ed_clients)
        return ENOMEM;

    for (i = 0; i < maxfds; i++)
    {
        epoll_obj->ed_clients[i].callback = NULL;
        epoll_obj->ed_clients[i].data = NULL;
        memset(epoll_obj->ed_clients[i].buffer, 0, REQUEST_SIZE);
        epoll_obj->ed_clients[i].buf_len = 0;
        epoll_obj->ed_clients[i].buf_parsed_len = 0;
        memset(&epoll_obj->ed_clients[i].http_req, 0, sizeof(epoll_obj->ed_clients[i].http_req));
    }

    /* create epoll object */
    epoll_obj->epoll_fd = epoll_create(maxfds);

    if (epoll_obj->epoll_fd == -1)
        return errno;

    epoll_obj->epoll_events = malloc(maxfds * sizeof(*epoll_obj->epoll_events));
    if (!epoll_obj->epoll_events)
        return ENOMEM;

    return 0;
}

void* work(void* arg)
{
    ed_threadpool_task *p = (ed_threadpool_task*) arg;
    //ed_client->callback(epoll_event, ed_client->data);
    if(p->ed_client != NULL && p->ed_client->callback != NULL)  
        p->ed_client->callback(p->epoll_event, p->ed_client->data);
    if(arg) free(arg);
}

/**
 * @brief Waits for epoll to generate events on some fds
 *        and then call callback function associated to them serially
 */
int ed_epoll_dispatch_events(struct ed_epoll *epoll_obj, int timeout)
{
    struct epoll_event *epoll_event = NULL;
    ed_client_t *ed_client = NULL;
    int i, nfds, event_fd;

    /* wait for events to generate */
    nfds = epoll_wait(epoll_obj->epoll_fd, epoll_obj->epoll_events, epoll_obj->maxfds, timeout);

    for (i = 0; i < nfds; i++)
    {
        epoll_event = &epoll_obj->epoll_events[i];
        event_fd = epoll_event->data.fd;
        if (event_fd < 0 || epoll_obj->fds_used <= 0)
        {
            dbg_printf("nfds %d. ignoring even on fd %d. fds in use %d", nfds, event_fd, epoll_obj->fds_used);
            continue;
        }

        ed_client = &epoll_obj->ed_clients[event_fd];
        if (ed_client == NULL || ed_client->callback == NULL)
        {
            err_printf("how come for event_fd: %d callback addr: %p\n", event_fd, &ed_client->callback);
        }

        //lsh  需要修改的地方，加入多线程  + epoll
        #if 0
        /* call callback function for this fd */
        ed_threadpool_task *tmp = (ed_threadpool_task*)calloc(1,sizeof(ed_threadpool_task));
        tmp->ed_client = ed_client;
        tmp->epoll_event = epoll_event;
        threadpool_add_job(pool, work, tmp);
        sleep(2);
        #else
        ed_client->callback(epoll_event, ed_client->data);
        #endif
    }

    return SUCCESS;
}

/**
 * @brief add a given fd into epoll fd set
 */
int ed_epoll_add(struct ed_epoll *epoll_obj, int fd, event_callback_func_t callback, void *data)
{
    struct epoll_event event;

    if (fd >= epoll_obj->maxfds)
    {
        err_printf("fd %d, increase maxfds value", fd);
        return FAILURE;
    }

    memset(&event, 0, sizeof(struct epoll_event));
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    if (epoll_ctl(epoll_obj->epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        err_printf("Error: could not add epoll event for fd %d. Code: %d\n", fd, errno);
        return errno;
    }

    epoll_obj->fds_used++;
    epoll_obj->ed_clients[fd].callback = callback;
    epoll_obj->ed_clients[fd].data = data;
    epoll_obj->ed_clients[fd].fd = fd;

    dbg_printf("adding new fd %d\n", fd);

    return SUCCESS;
}

/**
 * @brief change event set for given fd in epoll fd set
 */
int ed_epoll_set(struct ed_epoll *epoll_obj, int fd, int eventcode, void *data)
{
    struct epoll_event event;

    memset(&event, 0, sizeof(struct epoll_event));
    event.data.fd = fd;
    event.events = 0;
    event.events = eventcode | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    if (epoll_ctl(epoll_obj->epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        err_printf("could not modify events for socket %d, Code: %d\n", fd, errno);
        return FAILURE;
    }

    epoll_obj->ed_clients[fd].data = data;

    return SUCCESS;
}

/**
 * @brief delete a descriptor from epoll fd set
 */
int ed_epoll_del(struct ed_epoll *epoll_obj, int fd)
{
    struct epoll_event event;

    memset(&event, 0, sizeof(struct epoll_event));
    event.data.fd = fd;
    event.events = -1;

    /* delete this fd from epolli, it  */
    if (epoll_ctl(epoll_obj->epoll_fd, EPOLL_CTL_DEL, fd, &event) == -1)
    {
        err_printf("could not delete fd %d\n", fd);
        return FAILURE;
    }

    epoll_obj->fds_used--;
    epoll_obj->ed_clients[fd].callback = NULL;
    epoll_obj->ed_clients[fd].data = NULL;
    epoll_obj->ed_clients[fd].fd = 0;
    memset(epoll_obj->ed_clients[fd].buffer, 0, REQUEST_SIZE);
    epoll_obj->ed_clients[fd].buf_len = 0;
    epoll_obj->ed_clients[fd].buf_parsed_len = 0;
    memset(&epoll_obj->ed_clients[fd].http_req, 0, sizeof(epoll_obj->ed_clients[fd].http_req));

    dbg_printf("deleted socket %d from epoll\n", fd);

    return SUCCESS;
}
