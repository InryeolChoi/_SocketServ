#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 1)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	struct sockaddr_in addr;
	struct sockaddr_in addr2;
	addr.sin_addr.s_addr = htonl(0x1020304);
	addr2.sin_addr.s_addr = htonl(0x1010101);

	// inet_ntoa : 사용자가 동적 할당을 안 해도 됨.
	// 따라서 반환값을 미리 복사하는 것이 필요.
	char str_arr[20];
	char *str_ptr = inet_ntoa(addr.sin_addr);
	strcpy(str_arr, str_ptr);
	printf("addr : %s\n", str_ptr);

	inet_ntoa(addr2.sin_addr);
	printf("addr2 : %s\n", str_ptr); // addr이 아닌 addr2의 결과가 나옴.
	printf("addr : %s\n", str_arr);
}