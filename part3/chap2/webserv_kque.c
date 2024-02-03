#include "webserv.h"

void work_kqueue(int kq, int serv_sock)
{
	while (1)
	{
		struct kevent cur_events[MAX_EVENTS];
		// kq로 들어오는 이벤트를 cur_events 배열에 넣는다.
		int event_cnt = kevent(kq, NULL, 0, cur_events, MAX_EVENTS, NULL);
		if (event_cnt == -1)
			break ;

		for (int i = 0; i < event_cnt; i++)
		{
			int fd = cur_events[i].ident;
			if (fd == serv_sock)
				handle_clnt_connection(kq, serv_sock);
			else
				handle_clnt_response(fd);
		}
	}
}

void handle_clnt_connection(int kq, int serv_sock)
{
	struct sockaddr_in clnt_adr;
	socklen_t clnt_len = sizeof(clnt_adr);

	int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_len);
	if (clnt_sock == -1)
		return ;

	// 필터링 : 클라이언트 소켓에서 read를 잡도록 설정
	struct kevent clnt_event;
	EV_SET(&clnt_event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);

	// clnt_event에 걸리는 것 잡기
	if (kevent(kq, &clnt_event, 1, NULL, 0, NULL) == -1)
	{
		perror("failed to register\n");
		close(clnt_sock);
	}
}

void handle_clnt_response(int clnt_sock)
{
	char request_line[1024];
    FILE *clnt_file = fdopen(clnt_sock, "r");
    FILE *file_to_send = fdopen(dup(clnt_sock), "w");

    if (clnt_file == NULL || file_to_send == NULL)
        return ;

	// 요청메시지가 제대로 들어왔는지 확인.
	fgets(request_line, 1024, clnt_file);
	printf("%s", request_line);

	// 파싱 : 소켓 속 html 요청메시지
	char tmpline[1024];
	char method[1024];
	char filename[1024];
	char content[1024];

	strcpy(tmpline, request_line);
	strcpy(method, strtok(request_line, " /"));
	strcpy(filename, strtok(NULL, " /"));
	strcpy(content, get_content_name(filename));

	// 검사 : 제대로 된 요청인가?
	// 요청메시지가 http로 시작하는가? 메소드가 GET인가?
	if (strcmp(method, "GET") != 0 || access(filename, F_OK) == -1 || strstr(tmpline, "HTTP/") == NULL)
		send_err(file_to_send);
	else
		send_data(file_to_send, content, filename);

	// 무조건 소켓을 닫아줘야 segfault 방지
	fclose(file_to_send);
	close(clnt_sock);
}

char *get_content_name(char *file)
{
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];

	strcpy(file_name, file);
	strtok(file_name, ".");

	strcpy(extension, strtok(NULL, "."));
	if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
		return "text/html";
	else
		return "text/plain";
}