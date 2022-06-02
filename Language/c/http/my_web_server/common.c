#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#include "common.h"

/* Set Signal Handlers */
void set_signal_handlers()
{
    /*
     * We ignore SIGPIPE as there is no way to know that which Thread
     * hence which sockets has thrown ERROR. To gracefully handle this error
     * we ignore SIGPIPE here so write on socket throws EPIPE which we trap.
     */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        err_printf("Unable to set up signal handler for SIGPIPE.\n");
        exit(1);
    }
}

/* Start Server  and Listen for incoming requests */
int start_server_socket(int server_port)
{
    struct addrinfo hints, *res, *servinfo;
    int sockfd, yes;
    int error_code;
    char service[6];

    /* reset hint data structure */
    if (memset(&hints, 0, sizeof(hints)) == NULL)
    {
        fprintf(stderr, "memset: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
        return FAILURE;
    }

    /* reset service data structure */
    if (memset(&service, 0, sizeof(service)) == NULL)
    {
        fprintf(stderr, "memset: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
        return FAILURE;
    }
    sprintf(service, "%d", server_port);

    /* prepare hint data structure for getaddrinfo */
    hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    /* populate res data structure */
    if ((error_code = getaddrinfo(NULL, service, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: Failed. Code: %d, Error: %s\n", error_code, gai_strerror(error_code));
        return FAILURE;
    }

    /* loop through all the results and bind to the first we can */
    for (res = servinfo; res != NULL; res = res->ai_next)
    {
        /* create a server socket  */
        if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            fprintf(stderr, "socket: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
            continue;
        }

        /* set socket configuration */
        yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            close_socket(sockfd);
            fprintf(stderr, "setsockopt: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
            return FAILURE;
        }

        /* bind this socket  */
        if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0)
        {
            close_socket(sockfd);
            fprintf(stderr, "bind: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
            continue;
        }

        /* we reached here, we have valid socket */
        break;
    }

    if (res == NULL)
    {
        fprintf(stderr, "Could not create and bind server socket.\n");
        return FAILURE;
    }

    freeaddrinfo(servinfo); // all done with this structure

    /* listen on this socket */
    if (listen(sockfd, SERVERQUEUELENGTH) != 0)
    {
        close_socket(sockfd);
        fprintf(stderr, "listen: Failed. Code: %d, Error: %s\n", errno, strerror(errno));
        return FAILURE;
    }

    /* succefully strarted server socket */
    dbg_printf("Server succesfully started on port: %d\n", server_port);

    return sockfd;
}

/**
 * @brief closes socket safely
 */
void close_socket(int sockfd)
{
    if (close(sockfd) != 0)
    {
        fprintf(stderr, "close: Failed. Could not close server socket %d. Code: %d, Error: %s\n", sockfd, errno, strerror(errno));
    }
}

/**
 * @brief writes HTTP response header on a given fd
 *        for a given mime type and code
 */
int write_http_response_header(int fd, char *type, int size, int code, char *code_str)
{
    char header_buffer[1024] = {0};
    char curr_time[128] = {0};
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    strftime(curr_time, sizeof(curr_time), "%a, %d %b %Y %H:%M:%S %Z", tmp);
    sprintf(header_buffer, "HTTP/1.0 %d %s\n"
                           "Date: %s\n"
                           "Content-type: %s\n"
                           "Content-length: %d\n\n",
            code, code_str, curr_time, type, size);
    send(fd, header_buffer, strlen(header_buffer), 0);

    return SUCCESS;
}

/**
 * @brief write http response data on a desciptor
 */
int write_http_response_data(int fd, char *buffer, int size)
{
    send(fd, buffer, size, 0);

    return SUCCESS;
}

/**
 * @brief writes http response error for a given error code
 */
int write_http_response_error(int err_code, int fd)
{
    char *err_header;
    char *err_output;
    int size;

    switch (err_code)
    {
    case 404:
        err_header = "Not found";
        err_output = "Page Not Found";
        break;
    case 500:
    default:
        err_header = "Internal Server Error";
        err_output = "Internal Server Error";
    }

    size = strlen(err_output);
    write_http_response_header(fd, "text/plain", size, err_code, err_header);
    write_http_response_data(fd, err_output, size);

    return SUCCESS;
}
