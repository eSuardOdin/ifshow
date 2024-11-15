#ifndef SOCK_FUNCTIONS_H
#define SOCK_FUNCTIONS_H
#include "../../common/conf_sock.h"

int run_server(conf_sock socket_conf, char *buffer);
int handle_client(int sock);


#endif