#include "ks_server_init.h"

int socket_bind_listen(char *servip, int servport)
{
    int servfd;
    struct sockaddr_in servaddr;

    servfd = socket(AF_INET, SOCK_STREAM, 0);
    if(servfd == -1)
    {
        fprintf(stderr, "socket error\n");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if(inet_pton(AF_INET, servip, &servaddr.sin_addr) <= 0)
    {
        fprintf(stderr, "inet_pton error\n");
    }
    servaddr.sin_port = htons(servport);
    if(bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        fprintf(stderr, "bind error\n");
    }

    listen(servfd, LISTENQ);

    return servfd;
}
