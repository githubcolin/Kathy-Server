#include "kathy_server.h"
#include "ks_server_init.h"
#include "ks_epoll_module.h"


int main(int argc, char *argv[])
{
    int servfd;

    servfd = socket_bind_listen((char *)SERV_IP, SERV_PORT);

    ks_event_processes(servfd);

    return 0;
}
