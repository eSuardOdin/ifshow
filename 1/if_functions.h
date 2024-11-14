#ifndef IF_FUNCTIONS_H
#define IF_FUNCTIONS_H

#include <ifaddrs.h>
#include <sys/socket.h>


struct if_info
{
    char ifname[64];
    char **v6_addrs;
    char **v4_addrs;
    int v4_nb;
    int v6_nb;
};


/**
 * @brief Checks if an interface is already included in the tab
 * 
 * @param tab Tab of struct if_info
 * @param ifname ifname that is checked to see if we include it in tab
 * @param count tab count
 * @return int 
 */
int is_interface_included_struct(struct if_info *tab, char* ifname, int count);

struct if_info* get_if_info(struct if_info *tab, char* ifname, int count);


// int is_interface_included(char** tab, char* ifname, int count);

void show_console_usage();
int init_struct_tab(struct if_info *tab, struct ifaddrs *interfaces, unsigned char is_all_interfaces, char *searched_if, char *message);
int add_address(struct ifaddrs *addr, struct if_info *ifc);
int get_if_addresses(struct if_info *tab, struct ifaddrs *interfaces);
int format_result(struct if_info *tab, int tab_len, char *buf);

#endif