#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define IPADDR "127.0.0.1"
#define PORT 8787
#define LISTENQ 5
#define MAX_EVENTS 1024
#define MAXSIZE 1024

typedef struct 
{
    char *method;
    char *uri;
}http_request_header;

/*typedef struct*/
/*{*/
    /*char */
/*}*/

static int init_socket();
static void do_use_epoll(int listenfd);
static void do_accept_fd(int listenfd, int epollfd);
static void do_use_fd(int connfd);
static http_request_header *do_parse_request(char *buf);
static void do_use_request(int connfd, http_request_header *request);

int main(int argc, char *argv[])
{
    int listenfd;

    listenfd = init_socket();

    listen(listenfd, LISTENQ);

    do_use_epoll(listenfd);

    return 0;
}

static int init_socket()
{
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, IPADDR, &servaddr.sin_addr);
    /*servaddr.sin_addr.s_addr = htonl(INADDR_ANY);*/
    servaddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    return listenfd;
}

static void do_use_epoll(int listenfd)
{
    int epollfd;
    int nfds, n;
    struct epoll_event ev, events[MAX_EVENTS];
    
    epollfd = epoll_create(10);
    if(epollfd == -1)
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
    {
        perror("epoll_ctl: listenfd");
    }

    for(;;)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if(nfds == -1)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for(n = 0; n < nfds; n++)
        {
            if(events[n].data.fd == listenfd)
            {
                do_accept_fd(listenfd, epollfd);
            }
            else
            {
                do_use_fd(events[n].data.fd);
            }
        }
    }
}

static void do_accept_fd(int listenfd, int epollfd)
{
    int connfd;
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    struct epoll_event ev;

    clilen = 1; //if not initiate clilen will be accept: invalid argument.
    connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
    if(connfd == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    ev.events = EPOLLIN;
    ev.data.fd = connfd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1)
    {
        perror("epoll_ctl: connfd");
        exit(EXIT_FAILURE);
    }
}

static void do_use_fd(int connfd)
{
    char buf[MAXSIZE];
    int n;
    memset(buf, 0, MAXSIZE);
    n = read(connfd, buf, sizeof(buf));
    if(n == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    http_request_header *request = do_parse_request(buf);
    do_use_request(connfd, request);
}

static http_request_header *do_parse_request(char *buf)
{
    http_request_header *request;
    request = (http_request_header *)malloc(sizeof(http_request_header));
    if(request == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    request->method = strtok(buf, " ");
    request->uri = strtok(0, " ");
    return request;
}

static void do_use_request(int connfd, http_request_header *request)
{
    char file[64];
    sprintf(file, "../www%s", request->uri);

    FILE *fp = fopen(file, "rb");
    if(fp == 0)
    {
        char response[] = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
        write(connfd, response, sizeof(response));
    }
    else
    {
        int file_size;
        char content[1024];
        char headers[1024];
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n", file_size);
        send(connfd, headers, strlen(headers), 0);
        while(fread(content, 1, 1024, fp))
        {
            send(connfd, content, 1024, 0);
        }
    }
    free(request);
    close(connfd);
}
