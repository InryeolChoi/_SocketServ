#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	int serv_socket;
	int clnt_socket;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr; 
	socklen_t clnt_addr_size;

	char message[] = "hello world";

	if (ac != 2)
	{
		printf("Usage : %s <port>\n", av[0]);
		exit(1);
	}

	serv_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_socket == -1)
		error_handling("Socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(av[1]));

	if (bind(serv_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	
	if (listen(serv_socket, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_socket = accept(serv_socket, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
	if (clnt_socket == -1)
		error_handling("accept() error");
	
	write(clnt_socket, message, sizeof(message));
	close(clnt_socket);
	close(serv_socket);
	return 0;
}