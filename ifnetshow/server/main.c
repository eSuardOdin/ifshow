#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "./agent/agent.h"
#include "../common/conf_sock.h"




int main(int argc, char const* argv[])
{

	struct sockaddr_in address;
	address.sin_family = AF_INET; // ipv4
	address.sin_addr.s_addr = INADDR_ANY; // Getting all network interfaces
	address.sin_port = htons(7070);
	conf_sock cs = {
		.domain = AF_INET,
		.type = SOCK_STREAM,
		.protocol = 0,
		address,
		sizeof(address),
		4
	};


	int connected_socket = run_server(cs);

	return 0;
}