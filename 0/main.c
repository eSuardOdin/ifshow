#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 2048


void show_help()
{
    printf("COMMAND USAGE: ifshow (-i <ifname> | -a)\n");
    printf("Options:\n");
    printf("\t-i <ifname> shows specified network interface IPv4 and IPv6 addresses\n");
    printf("\t-a display all network interfaces IPv4 and IPv6 addresses\n");
}

int is_interface_included(char** tab, char* ifname, int count)
{
    if(!count) return 0;
    for (int i = 0; i < count; i++) {
        // printf("Comparing %s with %s\n", tab[i], ifname);
        if(!strcmp(tab[i], ifname)) return 1;
    }
    return 0;
}


char * ifshow(const char * ifname, unsigned char is_all_interfaces, int count)
{
    char* p_result = malloc(BUFFER_SIZE);

    if(!p_result)
    {
        fprintf(stderr, "Buffer not allocated correctly in ifshow\n");
        return NULL;
    }
    else
    {
        // If error, return display help
        if(
            count < 2 ||
            ((!is_all_interfaces && count < 3) &&
            is_all_interfaces)
        )
        {
            strcat(p_result, "COMMAND USAGE: ifshow (-i <ifname> | -a)\nOptions:\n\t-i <ifname> shows specified network interface IPv4 and IPv6 addresses\n\t-a display all network interfaces IPv4 and IPv6 addresses\n");
        }
        else // If command is typed ok
        {
            // Setting ifnames array
            char** ifnames = NULL;
            int ifnb = 0;
            struct ifaddrs *interfaces, *ifa;
            
            // Get interfaces and exit if error
            if(getifaddrs(&interfaces) != 0)
            {
                fprintf(stderr, "Could not getifaddrs()\n");
                return NULL;
            }

            // iterate trough list
            for(ifa = interfaces; ifa->ifa_next != NULL; ifa = ifa->ifa_next)
            {
                if(ifa->ifa_next == NULL)
                    continue;
                // If (-i and interface name ok or all interfaces) and ifname not duplicate 
                if(
                    (!is_all_interfaces && !strcmp(ifname, ifa->ifa_name) || is_all_interfaces) &&
                    !is_interface_included(ifnames, ifa->ifa_name, ifnb))
                {
                    ifnames = (char **) realloc(ifnames, (size_t) (ifnb + 1) * sizeof(char *));
                    ifnames[ifnb] = ifa->ifa_name;
                    ifnb++;
                }
            }

            // If list still null, exit.
            if(ifnames == NULL)
            {
                if(is_all_interfaces)
                {
                    strcat(p_result, "No network interfaces to display\n");
                }
                else 
                {
                    strcat(p_result, "Interface not found\n"); 
                }
                return p_result;
            }

            for(int i = 0; i < ifnb; i++)
            {
                printf("%s\n", ifnames[i]);
            }
            
            // Free addresses
            freeifaddrs(interfaces);
        }
    }
    return p_result;
}

int main(int argc, char **argv)
{
    char * ifname = (argc > 2) ? argv[2] : "";
    unsigned char is_all_interfaces = (argc == 2 && !strcmp("-a", argv[1])) ? 1 : 0;


    char * display = ifshow(ifname, is_all_interfaces, argc);
    
    printf("%s", display);
    // // Checks if command syntax is ok
    // if(
    //     argc < 2 ||
    //     ((strcmp(argv[1], "-i") && argc < 3) &&
    //     strcmp(argv[1], "-a"))
    // )
    // {
    //     show_help();
    // }

    // else
    // {
    //     struct ifaddrs *interfaces, *ifa;
    //     char ip[INET6_ADDRSTRLEN];

    //     // Récupérer la liste des interfaces
    //     if (getifaddrs(&interfaces) == -1) {
    //         perror("getifaddrs");
    //         return EXIT_FAILURE;
    //     }

    //     for(ifa = interfaces; ifa->ifa_next != NULL; ifa = ifa->ifa_next)
    //     {
    //         if (ifa->ifa_addr == NULL) {
    //             continue;
    //         }
    //         // Check if IPV4
    //         if(ifa->ifa_addr->sa_family == AF_INET)
    //         {
    //             // Cast as an ipv4 addr
    //             struct sockaddr_in *addr = (struct sockaddr_in *) ifa->ifa_addr; 
    //             inet_ntop(AF_INET, &addr, ip, sizeof(ip));
    //             printf("Interface %s -> IPv4, addresse : %s\n", ifa->ifa_name, ip);
    //         }
    //         else if(ifa->ifa_addr->sa_family == AF_INET6)
    //         {
    //             // Cast as an ipv6 addr
    //             struct sockaddr_in6 *addr = (struct sockaddr_in6 *) ifa->ifa_addr; 
    //             inet_ntop(AF_INET6, &addr, ip, sizeof(ip));
    //             printf("Interface %s -> IPv6, addresse : %s\n", ifa->ifa_name, ip);
    //         }
    //     }


    //     freeifaddrs(interfaces);
    // }

    return EXIT_SUCCESS;
}