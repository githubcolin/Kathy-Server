#ifndef _KS_EPOLL_MODULE_H_
#define _KS_EPOLL_MODULE_H_


#include "ks_linux_config.h"

#define MAX_EVENTS 10

struct epoll_event ev, events[MAX_EVENTS];

void ks_event_processes(int servfd);


#endif
