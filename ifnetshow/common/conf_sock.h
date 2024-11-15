#ifndef CONF_SOCK_H
#define CONF_SOCK_H

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

typedef struct conf_sock
{
    int domain;
    int type;
    int protocol;
    struct sockaddr_in address;
    int address_len;
    int queue;
} conf_sock;

#endif