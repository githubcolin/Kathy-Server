#include "ks_http_module.h"

void ks_connection_accept(int servfd, int epollfd)
{
    int connfd;
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    struct epoll_event ev;

    connfd = accept(servfd, (struct sockaddr *)&cliaddr, &clilen);
    if(connfd == -1)
    {
        fprintf(stderr, "accept error\n");
        exit(EXIT_FAILURE);
    }

    ev.events= EPOLLIN;
    ev.data.fd = connfd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1)
    {
        fprintf(stderr, "epoll_ctl error\n");
        exit(EXIT_FAILURE);
    }
}

void ks_connection_processes(int connfd)
{
    char buf[1024];
    if(read(connfd, buf, 1024) == -1)
    {
        fprintf(stderr, "read error\n");
        exit(EXIT_FAILURE);
    }

    ks_http_request_header *khrh = ks_http_request_header_init();

    ks_http_request_header_parse(buf, khrh);

    ks_http_response(connfd, khrh);
}

ks_http_request_header *ks_http_request_header_init()
{
    ks_http_request_header *khrh;
    
    khrh = (ks_http_request_header *)malloc(sizeof(ks_http_request_header));
    if(khrh == NULL)
    {
        fprintf(stderr, "malloc error\n");
        exit(EXIT_FAILURE);
    }
    
    return khrh;
}


void ks_http_request_header_parse(char *buf, ks_http_request_header *khrh)
{
    khrh -> method = strtok(buf, " ");
    khrh -> uri = strtok(0, " ");
}

void ks_http_response(int connfd, ks_http_request_header *khrh)
{
    char file[64];
    FILE *fp;
    sprintf(file, "../www%s", khrh -> uri);

    fp = fopen(file, "rb");
    if(fp == 0)
    {
        char response[] = "HTTP/1.1 404 NOT FOUND\r\n\r\n";
        send(connfd, response, sizeof(response), 0);
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
}
