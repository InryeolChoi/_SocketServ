#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		printf("usage : ./%s\n", av[0]);
		exit(0);
	}

	int sock, res;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
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
		
		str_len = write(sock, message, strlen(message));
		recv_len = 0;

		// 변경구간 : read()가 데이터를 전부 받도록 수정.
		while (recv_len < str_len)
		{
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			if (recv_cnt == -1)
				error_handling("read() error");
			recv_len += recv_cnt;
		}
	}
	close(sock);
	return 0;
}