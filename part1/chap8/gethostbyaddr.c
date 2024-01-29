#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage : %s <addr>\n", av[0]);
		exit(1);
	}
	struct hostent *host;
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(av[1]);
	host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
	if (!host)
		error_handling("gethost() error");

	printf("Official name : %s\n", host->h_name);
	for (int i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i+1, host->h_aliases[i]);
	
	if (host->h_addrtype == AF_INET)
		printf("Address type: AF_INET\n");
	else if (host->h_addrtype == AF_INET6)
		printf("Address type: AF_INET6\n");
	else
		printf("Address type: others\n");

	for (int i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
	
	return 0;
}