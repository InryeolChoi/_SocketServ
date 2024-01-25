#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 1)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	char *addr = "1.2.3.4";
	struct sockaddr_in addr_inet;

	int result = inet_aton(addr, &addr_inet.sin_addr);
	if (!result)
		error_handling("conversion error");
	else
	{
		printf("네트워크 바이트 순서대로 정렬된 정수 주소 : ");
		printf("%#x\n", addr_inet.sin_addr.s_addr);
	}
	return 0;
}