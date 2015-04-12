#ifndef _KS_HTTP_MODILE_H_
#define _KS_HTTP_MODILE_H_


#include "ks_linux_config.h"


typedef struct{
    char *method;
    char *uri;
}ks_http_request_header;


void ks_connection_accept(int servfd, int epollfd);

void ks_connection_processes(int connfd);

ks_http_request_header *ks_http_request_header_init();

void ks_http_request_header_parse(char *buf, ks_http_request_header *khrh);

void ks_http_response(int connfd, ks_http_request_header *khrh);

#endif
