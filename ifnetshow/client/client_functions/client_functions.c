#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "./client_functions.h"

void show_command_usage()
{
    printf("COMMAND USAGE: ifnetshow -n <address> (-i <ifname> | -a)\n");
    printf("Options:\n");
    printf("\t-n <address> one of the network interface address of target machine\n");
    printf("\t-i <ifname> shows specified network interface IPv4 and IPv6 addresses\n");
    printf("\t-a display all network interfaces IPv4 and IPv6 addresses\n");
}


int check_command_input(int argc, char **argv, int *net_family)
{
    // ---- Check arguments number ----
    if(argc < 4 || argc > 5)
    {
        show_command_usage();
        return -1;
    }

    // ---- Check if address is formatted right ----
    struct in6_addr v4;
    struct in6_addr v6;
    if(inet_pton(AF_INET,  argv[2], &v4) == 1)
    {
        *net_family = AF_INET6;
    }
    if(!*net_family && inet_pton(AF_INET6,  argv[2], &v6) == 1)
    {
        *net_family = AF_INET6;
    }

    if(!*net_family)
    {
        show_command_usage();
        return -1;
    }

    // Check options
    if((argc == 4 && strcmp(argv[3], "-a")) || (argc == 5 && strcmp(argv[3], "-i")) )
    {
        show_command_usage();
        return -1;
    }
    return 0;
}



// int serv_connect(int sock, int port, char *addr, int *net_family)
// {
//     printf("Entering");
//     int connect_sock;
//     switch (*net_family) {
//         case AF_INET:
//             struct sockaddr_in serv_addr;
//             serv_addr.sin_port = htons(port);
//             serv_addr.sin_family = AF_INET;
//             inet_pton(AF_INET, addr, &serv_addr.sin_addr);
//             if((connect_sock = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) == -1)
//             {
//                 return -1;
//             }
//             break;
//         case AF_INET6:
//             struct sockaddr_in6 serv_addr6;
//             serv_addr6.sin6_port = htons(port);
//             serv_addr6.sin6_family = AF_INET6;
//             inet_pton(AF_INET6, addr, &serv_addr6.sin6_addr);
//             if((connect_sock = connect(sock, (struct sockaddr *) &serv_addr6, sizeof(serv_addr6))) == -1)
//             {
//                 return -1;
//             }
//             break;
//     }
//     printf("Exiting");
//     return connect_sock;
// }