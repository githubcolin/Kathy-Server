EXEC = kathy_server
CC = gcc
CFLAGS = -c -Wall -O -g

$(EXEC) : kathy_server.o ks_server_init.o ks_epoll_module.o ks_http_module.o
	$(CC) -o $(EXEC) kathy_server.o ks_server_init.o ks_epoll_module.o ks_http_module.o

kathy_server.o : kathy_server.h kathy_server.c
	$(CC) $(CFLAGS) kathy_server.c

ks_server_init.o : ks_server_init.h ks_server_init.c
	$(CC) $(CFLAGS) ks_server_init.c

ks_epoll_module.o : ks_epoll_module.h ks_epoll_module.c
	$(CC) $(CFLAGS) ks_epoll_module.c

ks_http_module.o : ks_http_module.h ks_http_module.c
	$(CC) $(CFLAGS) ks_http_module.c

clean :
	rm -rf *.o $(EXEC)
