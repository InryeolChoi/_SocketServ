#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 1)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned long host_addr = 0x12345678;
	unsigned long net_addr;

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("<포트>\n");
	printf("호스트 바이트 순서 : %#x \n", host_port);
	printf("네트워크 바이트 순서 : %#x \n", net_port);

	printf("<ip주소>\n");
	printf("호스트 바이트 순서 : %#lx \n", host_addr);
	printf("네트워크 바이트 순서 : %#lx \n", net_addr);
	return 0;
}