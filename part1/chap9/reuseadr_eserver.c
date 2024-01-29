#include "../../socket_header.h"
#include "../../socket_header.c"

#define TRUE 1
#define FALSE 0

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage : %s <addr>\n", av[0]);
		exit(1);
	}

	int serv_sock, clnt_sock, res, str_len;
	char message[30];
	socklen_t clnt_adr_size;
	struct sockaddr_in serv_adr, clnt_adr;
	int option; socklen_t optlen;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	
	// 바로 재실행이 가능하도록 SO_REUSEADDR 옵션 변경
	optlen = sizeof(option);
	option = TRUE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	res = bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("bind() error");
	res = listen(serv_sock, 5);
	if (res == -1)
		error_handling("listen() error");
	clnt_adr_size = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);

	while ((str_len = read(clnt_sock, message, sizeof(message))) != 0)
	{
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}
	close(clnt_sock);
	close(serv_sock);
	return 0;
}