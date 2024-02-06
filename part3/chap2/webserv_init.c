#include "webserv.h"

int init_serv_sock(char *port)
{
	int serv_sock;
	int enable_reuse = 1;

	// 소켓 설계
	if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		error_handling("socket() error");

	// 서버 만들기 & 종료 후 포트 즉시 재사용 설정
	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int)) < 0)
	{
        perror("setsockopt(SO_REUSEADDR) failed");
        close(serv_sock);
        exit(EXIT_FAILURE);
	}

	// 포트 및 ip설정 (지금은 단일포트)
	struct sockaddr_in serv_adr;

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(port));

	// bind()와 listen()
	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	return (serv_sock);
}

int init_kqueue(int serv_sock)
{
	int kq;

	// kqueue 설정
	if ((kq = kqueue()) == -1)
		error_handling("kqueue() error");

	// serv_event 구조체의 초기화 : 읽기 이벤트 감시 활성화.
	struct kevent serv_event;
    EV_SET(&serv_event, serv_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);

	return kq;
}

void catch_read_kqueue(int kq, int serv_sock)
{
    struct kevent serv_event;
    EV_SET(&serv_event, serv_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);

	// 필터링
	if (kevent(kq, &serv_event, 1, NULL, 0, NULL) == -1)
	{
		perror("Failed to register server socket with kqueue");
		close(serv_sock);
		close(kq);
        exit(EXIT_FAILURE);
	}
}
