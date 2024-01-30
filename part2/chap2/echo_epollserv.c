#include "../../socket_header.h"
#include "../../socket_header.c"

#define BUF_SIZE2 100
#define EPOLL_SIZE 50

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : %s\n", av[0]);
		exit(1);
	}

	int serv_sock, clnt_sock, res;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_size;
	int str_len;
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

	struct epoll_event *ep_events;	//
	struct epoll_event *event;		//
	int epfd, event_cnt;

	// epoll_create
	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event *) * EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	// epfd에 추가(CTL_ADD), serv_sock을, event를 통해 전달된 이벤트의 관찰이 목적
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event); 

	while (1)
	{
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if (event_cnt == -1)
		{
			puts("epoll_wait() error");
			break ;
		}

		for (int i = 0; i < event_cnt, i++)
		{
			if (ep_events[i].data.fd == serv_sock)
			{
				adr_size = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_size);
				event.events = EPOLLIN;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("connected client : %d\n", clnt_sock);
			}
			else
			{
				str_len = read(ep_events[i].data.fd, buf, BUF_SIZE2);
				if (str_len == 0)
				{
					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client : %d\n", ep_events[i].data.fd);
				}
				else
					write(ep_events[i].data.fd, buf, str_len);
			}
		}
	}
	close(serv_sock);
	close(epfd);

	return 0;
}