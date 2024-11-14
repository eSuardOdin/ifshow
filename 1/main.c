#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "./if_functions.h"

#define BUFFER_SIZE 2048





int main(int argc, char** argv)
{
    if(
        argc < 2 ||
        !((!strcmp(argv[1], "-a") && argc == 2) || (!strcmp(argv[1], "-i") && argc == 3))
    )
    {
        show_console_usage();
        return 0;
    }

    // Tab of interfaces info
    struct if_info *if_tab = NULL;
    int tab_len;
    // Option -a -> 1
    unsigned char is_all_interfaces;
    // Interfaces linked list
    struct ifaddrs *interfaces;


    char *message = malloc(BUFFER_SIZE);
    char *searched_if = (argc == 3) ? argv[2] : "";
    is_all_interfaces = !strcmp(argv[1], "-a") ? 1 : 0;



    // Get interfaces and exit if error
    if(getifaddrs(&interfaces) != 0)
    {
        fprintf(stderr, "Could not getifaddrs()\n");
        return -1;
    }

    // Init tab with all struct interfaces
    if((tab_len = init_struct_tab(if_tab, interfaces, is_all_interfaces, searched_if, message)) == -1)
    {
        printf("Error : %s\n", message);
        return -1;
    }
    
    // format_result(if_tab, tab_len, message);

    // Free addresses
    freeifaddrs(interfaces);
    return 0;
}


