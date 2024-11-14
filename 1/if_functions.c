
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "./if_functions.h"


int init_struct_tab(struct if_info* tab, struct ifaddrs *interfaces, unsigned char is_all_interfaces, char *searched_if, char *message)
{
    struct ifaddrs *ifa;
    int ifnb = 0;
    // iterate trough list
    for(ifa = interfaces; ifa->ifa_next != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_next == NULL)
            continue;
        // If (-i and interface name ok or all interfaces) and ifname not duplicate 
        if(
            ((!is_all_interfaces && !strcmp(searched_if, ifa->ifa_name)) || is_all_interfaces) &&
            !is_interface_included_struct(tab, ifa->ifa_name, ifnb))
        {
            struct if_info ifc;
            strcpy(ifc.ifname, ifa->ifa_name);
            tab = (struct if_info *) realloc(tab, (size_t) (ifnb + 1) * sizeof(struct if_info));
            if(tab == NULL)
            {
                strcat(message, "Error in memory allocation\n");
                return -1;
            }
            tab[ifnb] = ifc;
            ifnb++;
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

    for(int i = 0; i < ifnb; i++)
    {
        printf("%s\n", tab[i].ifname);
    }
    return ifnb;
}


int is_interface_included_struct(struct if_info *tab, char* ifname, int count)
{
    if(!count) return 0;
    for (int i = 0; i < count; i++) {
        // printf("Comparing %s with %s\n", tab[i], ifname);
        if(!strcmp(tab[i].ifname, ifname)) return 1;
    }
    return 0;
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
        // printf("Comparing %s with %s\n", tab[i], ifname);
        if(!strcmp(tab[i].ifname, ifname)) return &tab[i];
    }
    return NULL;
}



int get_if_addresses(struct if_info *tab, struct ifaddrs *interfaces)
{
    struct ifaddrs *ifa;
    // Loop from adresses
    for(ifa = interfaces; ifa->ifa_next != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr == NULL)
        {
            continue;
        }
        // Branch v4 / V6
        
    }
    // Format address
}




void show_console_usage()
{
    printf("COMMAND USAGE: ifshow (-i <ifname> | -a)\n");
    printf("Options:\n");
    printf("\t-i <ifname> shows specified network interface IPv4 and IPv6 addresses\n");
    printf("\t-a display all network interfaces IPv4 and IPv6 addresses\n");
}