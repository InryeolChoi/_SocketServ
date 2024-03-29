#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : %s\n", av[0]);
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
	
	// ip주소와 포트번호 입력
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	// bind()로 serv_sock에 ip/포트를 집어넣음.
	res = bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("bind() error");

	// listen()으로 연결 대기열 생성.
	res = listen(serv_sock, 5);
	if (res == -1)
		error_handling("listen() error");

	clnt_adr_size = sizeof(clnt_adr);

	// accept() 반복을 통한 지속적 응답
	// 5개의 클라이언트를 순서대로 응답.
	for (int i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_size);
		if (clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d\n", i + 1);
		
		// read()를 통해 message를 읽고, write()를 통해 message를 작성
		// 메시지가 안 오면(=클라이언트가 q를 누르면) str_len == 0으로 인식하고 종료
		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sock, message, str_len);

		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}