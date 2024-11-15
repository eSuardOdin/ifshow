#include <arpa/inet.h>
#include <bits/types/struct_iovec.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "./agent.h"

int handle_client(int sock)
{
    printf("Closing socket of client %d\n", sock);
    return 0;
}

// Check if the request is well formatted
// client:[ifnetshow -n addr] true request [-i ifname]
int handle_request(int sock, char *req, char *res)
{
    read(sock, req, (size_t)1024);
    
}


int run_server(conf_sock socket_conf, char *buffer)
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
	struct sockaddr_storage clients_addr;
    // char buffer[1024];
    char req[1024];
    char res[1024];
    while (1) {
        if((connected_socket = accept(sock, (struct sockaddr *) &socket_conf.address, (socklen_t*)&socket_conf.address_len)) == -1)
        {
            printf("Error while accepting connection : %s (errno: %d)\n", strerror(errno), errno);
            // return -1;
            continue;
        }
        printf("New connection !\n");
        // Read request
		read(connected_socket, buffer, (size_t)1024);
        printf("%s\n", buffer);
        // Send response
        sprintf(buffer, "Hello client, you have socket n°%d\n", connected_socket);
        send(connected_socket, buffer, (size_t)1024, 0);

        handle_client(connected_socket);
    }
        
    return connected_socket;
}