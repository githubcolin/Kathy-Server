#include "ks_epoll_module.h"
#include "ks_http_module.h"

void ks_event_processes(int servfd)
{
    int epollfd, nfds, n;

    epollfd = epoll_create(MAX_EVENTS);
    if(epollfd == -1)
    {
        fprintf(stderr, "epoll_create error\n");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = servfd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, servfd, &ev) == -1)
    {
        fprintf(stderr, "epoll_ctl error\n");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if(nfds == -1)
        {
            fprintf(stderr, "epoll_wait error\n");
            exit(EXIT_FAILURE);
        }

        for(n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == servfd )
            {
                ks_connection_accept(servfd, epollfd);
            }
            else
            {
                ks_connection_processes(events[n].data.fd);
            }
        }
    }
}












