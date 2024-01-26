#include "../../socket_header.h"
#include "../../socket_header.c"
#define BUF_SIZE 30

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : %s\n", av[0]);
		exit(0);
	}

	int serv_sd, clnt_sd;
	FILE *fp;
	char buf[BUF_SIZE];
	int read_cnt;

	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_size;

	// 클라이언트에게 file_server.c를 전송하기 위해 파일을 열고 있음.
	fp = fopen("file_server.c", "rb");

	serv_sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	bind(serv_sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);

	clnt_adr_size = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_adr, &clnt_adr_size);

	while (1)
	{
		// 파일 데이터를 전송하기 위한 느낌.
		read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);

		if (read_cnt >= BUF_SIZE)			// 계속 30씩 보내기
			write(clnt_sd, buf, BUF_SIZE);
		else if (read_cnt < BUF_SIZE)		// 다 읽고 남은 부분
		{
			write(clnt_sd, buf, read_cnt);
			break ;
		}
	}

	// 파일 전송 후 출력 스트림에 대한 half-close 진행
	shutdown(clnt_sd, SHUT_WR);

	// 출력 스트림만 닫은 상태, client로부터 입력은 받을 수 있음.
	read(clnt_sd, buf, BUF_SIZE);
	printf("message from client : %s \n", buf);

	// 파일과 소켓을 완전히 닫기.
	fclose(fp);
	close(clnt_sd);
	return 0;
}