/*

Lisez le code puis compilez le. Lancez le et vérifiez qu’il accepte bien une connection à
l’addresse http://127.0.0.1:7070/ via un navigateur ou cURL (Attention, comme le programme ne
renvoie pas de réponse, votre navigateur vous donnera une erreur. Pour vérifier que le
programme fonctionne, regardez le terminal).
2. Créez une structure qui contiendra toutes les informations relatives à la configuration de la
socket (addresses acceptées, protocole ipv4 ou 6, TCP ou UDP, port d’écoute, nombre maximum
de connections en attente). Cette structure pourra contenir un champ de type sockaddr_in.
3. Créez une fonction nommée « run_server » qui prend en argument la structure précédement
créée et qui
- Crée la socket
- Attache la socket à l’addresse précisée
- Demande à la socket d’écouter les connections.
- Accepte une connection et affiche le numéro de la socket créée
4. Modifiez la fonction run_server pour qu’elle accepte plusieurs connections. À chaque
connection, elle affichera le numéro de la socket créée dans la console puis elle la fermera.


*/


#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "./agent/agent.h"
#include "../common/conf_sock.h"
#include "./if_functions/if_functions.h"



int main(int argc, char const* argv[])
{
	char buffer[1024] = {0};

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


	int connected_socket = run_server(cs, buffer);
	if(connected_socket != -1)
	{
		printf("Socket connected : %d\n", connected_socket);
	}

	return 0;
}