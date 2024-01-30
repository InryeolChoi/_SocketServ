#include "../../socket_header.h"
#include "../../socket_header.c"
#define BUF_SIZE2 100

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : %s\n", av[0]);
		exit(1);
	}

	int serv_sock, clnt_sock, res;
	struct sockaddr_in serv_adr, clnt_adr;

	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_size;
	int fd_max, str_len, fd_num;
	char buf[BUF_SIZE2];

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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
	
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);		// 서버 소켓 등록
	fd_max = serv_sock;

	while (1)
	{
		cpy_reads = reads;			// 복사본을 떠야 한다.
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		// 현재 fd번호에 1개를 추가해야 전체 갯수가 나온다.
		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break ;
		if (fd_num == 0)
			continue ;

		// 여기서부턴 select의 반환값이 1 이상일때만 실행된다.
		for (int i = 0; i < fd_max + 1; i++)
		{
			// cpy_reads 중 i번째 fd가 그대로 1인지 검사. (= 변화가 있는 fd 잡아내기)
			if (FD_ISSET(i, &cpy_reads))
			{
				// i가 서버의 소켓이라면
				if (i == serv_sock)
				{
					adr_size = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_size);
					// reads에 클라이언트 소켓 등록.
					FD_SET(clnt_sock, &reads);

					// 만약 클라이언트 소켓이 fd_max보다 크다면 fd_max값 바꿔주기.
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					
					// 몇번 소켓인지 표기.
					printf("connected client : %d\n", clnt_sock);
				}
				// i가 서버의 소켓이 아니라면 (즉 클라이언트 소켓이라면)
				else
				{
					// 일단 read로 buf에 그 내용을 쓴다.
					str_len = read(i, buf, BUF_SIZE2);
					// 만약 EOF(길이가 0)이면 해당 클라이언트 소켓을 닫을 준비를 한다.				
					if (str_len == 0)
					{
						FD_CLR(i, &reads);	// reads에서 해당 소켓 삭제
						close(i);			// 소켓 닫기
						printf("closed client : %d\n", i);
					}
					// 아니면 buf를 출력한다.
					else
						write(i, buf, str_len);
				}
			}
		}
	}

	// 서버 소켓을 닫는다.
	close(serv_sock);
	return 0;
}