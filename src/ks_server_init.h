#ifndef _KS_SERVER_INIT_H_
#define _KS_SERVER_INIT_H_


#include "ks_linux_config.h"

#define LISTENQ 5

int socket_bind_listen(char *servip, int servport);


#endif
