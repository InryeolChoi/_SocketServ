#include "webserv.h"

int main(int ac, char **av)
{
	int serv_sock, kq;

	if (ac != 2)
	{
		printf("Usage : %s <port>\n", av[0]);
		exit(1);
	}

	// 서버소켓과 kq 등록
	serv_sock = init_serv_sock(av[1]);
	kq = init_kqueue(serv_sock);

	// 처음에 서버가 read만을 잡도록 설정.
	catch_read_kqueue(kq, serv_sock);

	// kqueue를 이용한 서버 굴리기
	printf("HTTP Server started. Listening on port %s\n", av[1]);
	work_kqueue(kq, serv_sock);
	close(serv_sock);
	close(kq);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

