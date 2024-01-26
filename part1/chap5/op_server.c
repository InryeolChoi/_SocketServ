#include "../../socket_header.h"
#include "../../socket_header.c"
#define OP_SIZE 4

int calculate(int opnum, int opnds[], char op);

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : %s\n", av[0]);
		exit(0);
	}

	int serv_sock, clnt_sock, res;
	char opinfo[BUF_SIZE];
	int result, opnd_cnt;
	int recv_cnt, recv_len;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	res = bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("bind() error");

	// 연결요청 대기 : 5칸
	res = listen(serv_sock, 5);
	if (res == -1)
		error_handling("listen() error");

	clnt_adr_size = sizeof(clnt_adr);

	// 5개의 클라이언트 처리
	for (int i = 0; i < 5; i++)
	{
		opnd_cnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
		// 피연산자 갯수
		read(clnt_sock, &opnd_cnt, 1);

		recv_len = 0;
		// 피연산자 갯수 -> char 단위로 변환
		while (recv_len < (opnd_cnt * OP_SIZE + 1))
		{
			// 배열에 담고, 길이를 반환
			recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		// 계산 후 전송
		result = calculate(opnd_cnt, (int *)opinfo, opinfo[recv_len - 1]);
		write(clnt_sock, (char *)&result, sizeof(result));
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

int calculate(int opnum, int opnds[], char op)
{
	int result = opnds[0];
	switch (op)
	{
	case '+':
		for	(int i = 1; i < opnum; i++)
			result += opnds[i];
		break;

	case '-':
		for	(int i = 1; i < opnum; i++)
			result -= opnds[i];
		break;

	case '*':
		for	(int i = 1; i < opnum; i++)
			result *= opnds[i];
		break;
	}
	return result;
}