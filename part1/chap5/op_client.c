#include "../../socket_header.h"
#include "../../socket_header.c"

#define RLT_SIZE 4	// 피연산자의 바이트 수
#define OP_SIZE 4	// 연산결과의 바이트 수

int main(int ac, char **av)
{
	if (ac != 3)
	{
		printf("usage : %s\n", av[0]);
		exit(0);
	}

	int sock, res;
	char opmsg[BUF_SIZE]; // 메모리 공간
	int result, opnd_cnt;
	struct sockaddr_in serv_adr;

	// 소켓 생성
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// ip, port 입력
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(av[1]);
	serv_adr.sin_port = htons(atoi(av[2]));

	// connect()
	res = connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("connect() error");
	else
		puts("connected");

	// 피연산자 집어넣을 횟수
	fputs("operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	// char로의 형변환
	opmsg[0] = (char)opnd_cnt;

	// 피연산자 입력하기
	for (int i = 0; i < opnd_cnt; i++)
	{
		printf("operand %d: ", i + 1);
		// char형 배열에 int형 정수를 저장하기 위한 부분
		// int = 4바이트, 따라서 index를 4씩 늘림.
		scanf("%d", (int *)&opmsg[i * OP_SIZE + 1]);
	}

	// 버퍼에 남아있는 '\n' 삭제를 위해 fgetc() 이용 
	fgetc(stdin);

	// 연산자 입력
	fputs("Operator : ", stdout);
	scanf("%c", &opmsg[opnd_cnt * OP_SIZE + 1]);

	// client -> server로 전송
	write(sock, opmsg, opnd_cnt * OP_SIZE + 2);

	// server -> client로 계산 결과 받기
	read(sock, &result, RLT_SIZE);

	// 출력
	printf("operation result: %d \n", result);
	close(sock);
	return 0;
}