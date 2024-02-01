#include "webserv.h"

int main(int ac, char **av)
{
	int serv_sock, kq;

	if (ac != 2)
	{
		printf("Usage : %s <port>\n", av[0]);
		exit(1);
	}

	// 서버소켓과 kq 등록
	serv_sock = init_serv_sock(av[1]);
	kq = init_kqueue(serv_sock);
	
	// 처음에 서버가 read만을 잡도록 설정.
	catch_read_kqueue(kq, serv_sock);

	// kqueue를 이용한 서버 굴리기
	printf("HTTP Server started. Listening on port %s\n", av[1]);
	work_kqueue(kq, serv_sock);
	close(serv_sock);
	close(kq);
}

void work_kqueue(int kq, int serv_sock)
{
	while (1)
	{
		struct kevent cur_events[MAX_EVENTS];
		// kq로 들어오는 이벤트를 events 배열에 넣는다.
		int event_cnt = kevent(kq, NULL, 0, cur_events, MAX_EVENTS, NULL);
		if (event_cnt == -1)
			break ;

		for (int i = 0; i < event_cnt; i++)
		{
			int fd = cur_events[i].ident;
			if (fd == serv_sock)
			{
				struct sockaddr_in clnt_adr;
				socklen_t clnt_len = sizeof(clnt_adr);

				int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_len);
				if (clnt_sock == -1)
					continue ;

				// 필터링 : 클라이언트 소켓에서 read를 잡도록 설정
				struct kevent clnt_event;
				EV_SET(&clnt_event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);

				// clnt_event에 걸리는 것 잡기
				if (kevent(kq, &clnt_event, 1, NULL, 0, NULL) == -1)
				{
					perror("failed to register\n");
					close(clnt_sock);
				}
				else
					printf("accept connection\n");
			}
			else
				response_to(fd);
		}
	}

}
