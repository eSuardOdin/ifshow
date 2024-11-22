#ifndef AGENT_H
#define AGENT_H
#include "../../common/conf_sock.h"

int run_server(conf_sock socket_conf/*, char *buffer*/);
int handle_client(int sock);


#endif