#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#define DEFAULT_URL  "http://192.168.6.17:8080"

void http_request_done(struct evhttp_request *req, void *arg)
{
    char buffer[256];
    int nread;

    if (req == NULL) 
    {
        printf("request failed\n");
        return;
    }
    fprintf(stderr, "Response line: %d %s\n", evhttp_request_get_response_code(req),req->response_code_line);

    while ((nread = evbuffer_remove(evhttp_request_get_input_buffer(req), buffer, sizeof(buffer)))  > 0) 
    {
        fwrite(buffer, nread, 1, stdout);
        fprintf(stderr, "%s\n",buffer);
    }
}

int main(int argc, char **argv)
{
    char *url = DEFAULT_URL;
    if (argc == 2) {
        url = argv[1];
    }
    
    struct evhttp_uri *http_uri = NULL;
    const char *scheme, *host, *path, *query;
    char uri[256];
    int port;

    http_uri = evhttp_uri_parse(url);
    
    scheme = evhttp_uri_get_scheme(http_uri);
    if (scheme == NULL || (strcasecmp(scheme, "https") != 0 && strcasecmp(scheme, "http") != 0)) 
    {
        printf("url must be http or https\n");
        return 1;
    }

    host = evhttp_uri_get_host(http_uri);
    port = evhttp_uri_get_port(http_uri);
    if (port == -1) 
    {
        port = (strcasecmp(scheme, "http") == 0) ? 8082 : 443;
    }

    path = evhttp_uri_get_path(http_uri);
    if (strlen(path) == 0) 
    {
        path = "/";
    }
    
    query = evhttp_uri_get_query(http_uri);
    if (query == NULL) 
    {
        snprintf(uri, sizeof(uri) - 1, "%s", path);
    } 
    else 
    {
        snprintf(uri, sizeof(uri) - 1, "%s?%s", path, query);
    }
    uri[sizeof(uri) - 1] = '\0';
    
    printf("url: %s\n", url);
    printf("scheme: %s\n", scheme);
    printf("host: %s\n", host);
    printf("port: %d\n", port);
    printf("path: %s\n", path);
    printf("uri: %s\n", uri);
    
    struct event_base *base;
    struct evhttp_connection *conn;
    struct evhttp_request *req;
    
    base = event_base_new();
    conn = evhttp_connection_base_new(base, NULL, host, port);
    req = evhttp_request_new(http_request_done, base);
    
    evhttp_add_header(req->output_headers, "Host", host);
    //evhttp_add_header(req->output_headers, "Connection", "close");
    evhttp_add_header(req->output_headers, "Connection", "Keep-Alive");
    
    evhttp_make_request(conn, req, EVHTTP_REQ_GET, uri);
    
    event_base_dispatch(base);
    
    return 0;
}