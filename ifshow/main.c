#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "../if_functions/if_functions.h"

#define BUFFER_SIZE 2048





int main(int argc, char** argv)
{
    if(
        argc < 2 ||
        !((!strcmp(argv[1], "-a") && argc == 2) || (!strcmp(argv[1], "-i") && argc == 3))
    )
    {
        show_command_usage();
        return 0;
    }

    int nb_if;  // Option -a -> 1
    unsigned char is_all_interfaces;

    char *message = malloc(BUFFER_SIZE);
    char *searched_if = (argc == 3) ? argv[2] : "";
    is_all_interfaces = !strcmp(argv[1], "-a") ? 1 : 0;



    // Init tab with all struct interfaces
    if((nb_if = ifshow(is_all_interfaces, searched_if, message)) == -1)
    {
        printf("Error : %s\n", message);
        return -1;
    }
    
    printf("Result : \n%s", message);
    
    return 0;
}


