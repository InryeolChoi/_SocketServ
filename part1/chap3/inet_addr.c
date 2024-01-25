#include "../../socket_header.h"

int main(int ac, char **av)
{
	if (ac != 1)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	char *addr1 = "1.2.3.4";	// 정상적인 ip 주소
	char *addr2 = "1.2.3.256";	// 잘못된 ip 주소

	unsigned long conv_addr1 = inet_addr(addr1);
	if (conv_addr1 == INADDR_NONE)
		printf("Error\n");
	else
		printf("네트워크 바이트 순서대로 정렬된 정수 주소 : %#lx\n", conv_addr1);

	unsigned long conv_addr2 = inet_addr(addr2);
	if (conv_addr2 == INADDR_NONE)
		printf("Error\n");
	else
		printf("네트워크 바이트 순서대로 정렬된 정수 주소 : %#lx\n", conv_addr2);

	return 0;
}