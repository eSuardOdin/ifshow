
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


int format_result(struct if_info *tab, int tab_len, char *buf)
{
    char line[128]; 
    for(int i = 0; i < tab_len; i++)
    {
        // printf( "###############################\nInterface : %s\n", tab[i].ifname);
        sprintf(line, "Interface : %s\n", tab[i].ifname);
        strcat(buf, line);
        if(!(tab[i].v4_nb+tab[i].v6_nb))
        {
            // printf("No adresses for this interface\n");
            sprintf(line, "No adresses for this interface\n");
            strcat(buf, line);
        }
        if(tab[i].v4_nb)
        {
            // printf("\n---- IPv4 ----\n");
            sprintf(line, "\n---- IPv4 ----\n");
            strcat(buf, line);
            for(int j = 0; j < tab[i].v4_nb; j++)
            {
                // printf("- %s\n", tab[i].v4_addrs[j]);
                sprintf(line, "- %s\n", tab[i].v4_addrs[j]);
                strcat(buf, line);
            }
        }
        if(tab[i].v6_nb)
        {
            // printf( "---- IPv6 ----\n");
            sprintf(line, "---- IPv6 ----\n");
            strcat(buf, line);
            for(int j = 0; j < tab[i].v6_nb; j++)
            {
                // printf("- %s\n", tab[i].v6_addrs[j]);
                sprintf(line, "- %s\n", tab[i].v6_addrs[j]);
                strcat(buf, line);
            }
        }
        printf( "\n");
        sprintf(line, "\n");
        strcat(buf, line);
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param interfaces The struct ifaddrs*
 * @param is_all_interfaces 1 : -a, 0 : -i
 * @param searched_if Named of the searched interface if -i
 * @param message The buffer to send back 
 * @return int 
 */
int ifshow(unsigned char is_all_interfaces, char *searched_if, char *message)
{
    struct if_info *tab = NULL;
    struct ifaddrs *interfaces, *ifa;
    int ifnb = 0;
    struct if_info *p_ifc = NULL;


    // Get interfaces and exit if error
    if(getifaddrs(&interfaces) != 0)
    {
        fprintf(stderr, "Could not getifaddrs()\n");
        return -1;
    }

    // iterate trough list
    for(ifa = interfaces; ifa->ifa_next != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_next == NULL)
            continue;

        // Get interface from tab
        p_ifc = get_if_info(tab, ifa->ifa_name, ifnb);
        // If interface not already in tab
        if(p_ifc == NULL)
        {
            // If -i and name of the interface does not match with searched one
            if(!is_all_interfaces && strcmp(searched_if, ifa->ifa_name))
                continue;
            // If else, create new struct
            struct if_info ifc;
            ifc.v4_nb = 0;
            ifc.v4_addrs = NULL;
            ifc.v6_nb = 0;
            ifc.v6_addrs = NULL;
            strcpy(ifc.ifname, ifa->ifa_name);
            // Resize tab
            tab = (struct if_info *) realloc(tab, (size_t) (ifnb + 1) * sizeof(struct if_info));
            if(tab == NULL)
            {
                strcat(message, "Error in memory allocation\n");
                return -1;
            }
            // Add struct to tab and get pointer to it
            tab[ifnb] = ifc;
            ifnb++;
            p_ifc = &tab[ifnb];
        }

        // Add address to struct
        if(add_address(ifa, p_ifc) != 0)
        {
            sprintf(message, "Error while adding address to %s\n", p_ifc->ifname);
            return -1;
        }
    }

    // If list still null, exit.
    if(tab == NULL)
    {
        if(is_all_interfaces)
        {
            strcat(message, "No network interfaces to display\n");
        }
        else 
        {
            strcat(message, "Interface not found\n"); 
        }
        return -1;
    }

    format_result(tab, ifnb, message);
    return ifnb;
}




/**
 * @brief Checks in the table if an if_info is present (by name)
 * 
 * @param tab The tab to search in
 * @param ifname The name to check against
 * @param count The number of elements in the table
 * @return Pointer to the struct or NULL if absent 
 */
struct if_info* get_if_info(struct if_info *tab, char* ifname, int count)
{
    if(!count) return NULL;
    for (int i = 0; i < count; i++) {
        if(!strcmp(tab[i].ifname, ifname)) return &tab[i];
    }
    return NULL;
}



int add_address(struct ifaddrs *base_addr, struct if_info *ifc)
{
    char ip[INET6_ADDRSTRLEN+7];
    char cidr[5];
    unsigned int readable_mask = 0;

    // ----- Branch IPv4 -----
    if(base_addr->ifa_addr->sa_family == AF_INET) 
    {
        struct sockaddr_in *addr = (struct sockaddr_in *) base_addr->ifa_addr;
        unsigned int mask = ((struct sockaddr_in *) base_addr->ifa_netmask)->sin_addr.s_addr;
        // Formatting address to readable format
        inet_ntop(AF_INET, &addr->sin_addr,ip, sizeof(ip));
        // Formatting v4 mask
        while(mask)
        {
            readable_mask+=(mask & 1);
            mask >>= 1;
        }
        sprintf(cidr, "/%d", readable_mask);
        strcat(ip, cidr);
        

        // Resize v6 tab
        char **new_v4_addrs = (char **) realloc(ifc->v4_addrs, (ifc->v4_nb + 1) * sizeof(char *));
        ifc->v4_addrs = new_v4_addrs;
        if(ifc->v4_addrs == NULL)
        {
            return -1;
        }
        // Add ip address to interface and increment specific version counter
        ifc->v4_addrs[ifc->v4_nb] = strdup(ip);
        ifc->v4_nb++;

    }

    // ----- Branch IPv6 -----
    else if(base_addr->ifa_addr->sa_family == AF_INET6)
    {
        struct sockaddr_in6 *addr = (struct sockaddr_in6 *) base_addr->ifa_addr;
        // Mask of 16 bytes
        u_int8_t *mask = ((struct sockaddr_in6 *) base_addr->ifa_netmask)->sin6_addr.s6_addr;
        u_int8_t byte;

        // Formatting address to readable format
        inet_ntop(AF_INET6, &addr->sin6_addr,ip, sizeof(ip));
        // Formatting v6 mask
        for(int i = 0; i < 16; i++)
        {
            byte = mask[i];
            while(byte > 0)
            {
                if(byte & 1)
                {
                    readable_mask++;
                }
                byte /= 2;
            }
        }
        sprintf(cidr, "/%d", readable_mask);
        strcat(ip, cidr);
        

        // Resize v6 tab
        char **new_v6_addrs = (char **) realloc(ifc->v6_addrs, (ifc->v6_nb + 1) * sizeof(char *));
        ifc->v6_addrs = new_v6_addrs;
        if(ifc->v6_addrs == NULL)
        {
            return -1;
        }
        // Add ip address to interface and increment specific version counter
        ifc->v6_addrs[ifc->v6_nb] = strdup(ip);
        ifc->v6_nb++;
    }
    return 0;
}



/**
 * @brief Shows help for command instruction
 * 
 */
void show_command_usage()
{
    printf("COMMAND USAGE: ifshow (-i <ifname> | -a)\n");
    printf("Options:\n");
    printf("\t-i <ifname> shows specified network interface IPv4 and IPv6 addresses\n");
    printf("\t-a display all network interfaces IPv4 and IPv6 addresses\n");
}