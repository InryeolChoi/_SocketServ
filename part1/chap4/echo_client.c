#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	int sock, str_len, res;
	char message[30];
	struct sockaddr_in serv_adr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(av[1]);
	serv_adr.sin_port = htons(atoi(av[2]));

	res = connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("connect() error");
	else
		puts("connected");

	while (1)
	{
		fputs("input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break ;
		
		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		printf("message from server : %s", message);
	}
	close(sock);
	return 0;
}