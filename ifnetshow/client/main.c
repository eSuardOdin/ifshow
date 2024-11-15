#include <asm-generic/socket.h>
#include <bits/types/struct_iovec.h>
#include <complex.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "../common/conf_sock.h"
#include "client_functions/client_functions.h"

#define PORT 7070

int main(int argc, char ** argv)
{
    int net_family = 0;
    // Checks if command is well formatted
    if(check_command_input(argc, argv, &net_family) != 0)
    {
        return -1;
    }

    char buffer[1024] = {0};

    struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(7070);


    //  ----- Create socket ----- 
    int sock;
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Error while creating the socket\n");
        return -1;
    }

    // ----- Connect to server V0 ----- 
    int connect_sock = 0;
    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_family = AF_INET;
    // Format address
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    if((connect_sock = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) != 0 )
    {
        printf("Error while connecting the socket\n");
        return -1;
    }

    // Send data to server
    sprintf(buffer, "Hello srv\n");
    send(sock, buffer, (size_t)1024, 0);
    // Read response from server
    read(sock, buffer, 1024);
    printf("Received from server : %s\n", buffer);

    close(connect_sock);
    return 0;
}