#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

void show_command_usage();
int check_command_input(int argc, char **argv, int *net_family);
// int serv_connect(int sock, int port, char *addr, int *net_family);



#endif