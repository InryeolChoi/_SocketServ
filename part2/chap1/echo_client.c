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

	// 소켓 생성
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// ip주소와 포트번호 입력
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(av[1]);
	serv_adr.sin_port = htons(atoi(av[2]));

	// 연결 가능상태로 변경
	res = connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("connect() error");
	else
		puts("connected");

	// 입력 & 출력
	while (1)
	{
		fputs("input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break ;
		
		// write()를 통해 sock에 message를 쓴다. (echo 서버로 보내짐.)
		write(sock, message, strlen(message));
		// read()를 통해 서버가 적은 sock의 문자열을 읽는다. 
		str_len = read(sock, message, BUF_SIZE - 1);
		// 해당 문자열 출력
		message[str_len] = 0;
		printf("message from server : %s", message);
	}
	close(sock);
	return 0;
}