#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 1)
	{
		printf("Usage : %s\n", av[0]);
		exit(1);
	}
	int sock;
	int snd_buf = 1024 * 3;
	int rcv_buf = 1024 * 3;
	int state;
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	// 변경하기
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, sizeof(len));
	if (state == 1)
		error_handling("getsockopt() error");
	
	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, sizeof(len));
	if (state == 1)
		error_handling("getsockopt() error");


	// 확인하기
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, &len);
	if (state == 1)
		error_handling("getsockopt() error");

	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, &len);
	if (state == 1)
		error_handling("getsockopt() error");

	printf("input buffer size : %d\n", rcv_buf);
	printf("output buffer size : %d\n", snd_buf);
	return 0;
}