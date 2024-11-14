#ifndef IF_FUNCTIONS_H
#define IF_FUNCTIONS_H

#include <ifaddrs.h>


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
/**
 * @brief Shows help for command instruction
 * 
 */
void show_console_usage();

/**
 * @brief Here to init the interfaces struct table
 *
 * 
 * @param tab 
 * @param interfaces 
 * @param is_all_interfaces 
 * @param searched_if 
 * @param message 
 * @return int 
 */
int init_struct_tab(struct if_info* tab, struct ifaddrs *interfaces, unsigned char is_all_interfaces, char *searched_if, char *message);



int get_if_addresses(struct if_info *tab, struct ifaddrs *interfaces);

#endif