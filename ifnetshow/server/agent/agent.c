#include <arpa/inet.h>
//#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "./agent.h"
#include "../../../if_functions/if_functions.h"

int handle_client(int sock)
{
    printf("Closing socket of client %d\n", sock);
    return 0;
}


int run_server(conf_sock socket_conf/*, char *buffer*/)
{
    // Create socket
    int sock;
    if((sock = socket(socket_conf.domain, socket_conf.type, socket_conf.protocol)) <= 0)
    {
        printf("Error on socket creation : %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    // Bind socket
    if((bind(sock, (struct sockaddr *) &socket_conf.address, socket_conf.address_len)) != 0)
    {
        printf("Error while binding the socket : %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    // Listen (marks the socket as passive socket (will be used to accept() connections))
    if((listen(sock, socket_conf.queue)) != 0)
    {
        printf("Error while listening with socket : %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    printf("ifnetshow agent listening...\n");

    // Accept connection
    int connected_socket;
    char req[2048] = {0};
    char res[2048] = {0};
    while (1) {
        if((connected_socket = accept(sock, (struct sockaddr *) &socket_conf.address, (socklen_t*)&socket_conf.address_len)) == -1)
        {
            printf("Error while accepting connection : %s (errno: %d)\n", strerror(errno), errno);
            // return -1;
            continue;
        }
        printf("New connection !\n");
        // Read request
		read(connected_socket, req, (size_t)2048);
        // printf("%s\n", buffer);
        
        int option = (req[0] == '\0') ? 1 : 0; // Just for more readable ifshow call
        // Send response
        ifshow(option, req, res);
        
        send(connected_socket, res, (size_t)2048, 0);
        // Clear buffer
        memset(res, 0, 2048);
        handle_client(connected_socket);
    }
        
    return connected_socket;
}

