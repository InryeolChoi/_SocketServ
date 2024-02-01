#include "Webserv.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage : %s <port> \n", av[0]);
		exit(1);
	}

	Webserv serv;
	serv.serv_init();

	while (1)
	{
		
	}
}
