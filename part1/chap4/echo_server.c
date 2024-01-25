#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	int serv_sock, clnt_sock, res;
	char message[BUF_SIZE];
	int str_len;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	res = bind(serv_sock, (struct socket *)&serv_adr, sizeof(serv_adr) == -1);
	if (res == -1)
		error_handling("bind() error");

	res = listen(serv_sock, 5);
	if (res == -1)
		error_handling("listen() error");

	clnt_adr_size = sizeof(clnt_adr);

	for (int i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d\n", i + 1);
		
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sock, message, str_len);

		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}