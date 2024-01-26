#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		printf("usage : %s\n", av[0]);
		exit(0);
	}

	int sock, res;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt;
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

	// 입력받기
	fputs("operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	opmsg[0] = (char)opnd_cnt;

	for (int i = 0; i < 5; i++)
	{
		printf("operand %d: ", i + 1);
		scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
	}

	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
	write(sock, opmsg, opnd_cnt * OPSZ + 2);
	read(sock, &result, RLT_SIZE);

	printf("operation result: %d \n", result);
	close(sock);
	return 0;
}