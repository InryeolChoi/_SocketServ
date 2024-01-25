#include "../../socket_header.h"
#include "../../socket_header.c"

int main(int ac, char **av)
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len = 0;
	int idx = 0, read_len = 0;

	if (ac != 3)
	{
		printf("Usage : %s <ip> <port>\n", av[0]);
		exit(1);
	}

	// 소켓생성 (IPv4, 연결지향형, 디폴트)
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// 프로토콜 & ip주소 & 포트번호 지정
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(av[1]);
	serv_addr.sin_port = htons(atoi(av[2]));

	// 연결요청 시도
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");

	// read()를 이용한다 == 소켓 역시 파일 디스크립터의 일종.
	// 1바이트씩 데이터를 읽는 중. str_len에는 읽어드린 바이트 수 저장.
	while (1)
	{
		read_len = read(sock, &message[idx++], 1);
		if (read_len == 0)
			break ;
		if (read_len == -1)
			error_handling("read() error!");
		str_len += read_len;
	}

	printf("message from server: %s\n", message);
	printf("function read call count : %d\n", str_len);

	// 소켓 닫기
	close(sock);
	return 0;
}