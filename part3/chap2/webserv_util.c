#include "webserv.h"

void response_to(int clnt_sock)
{
	char request_line[1024];
    FILE *clnt_file = fdopen(clnt_sock, "r");
    FILE *file_to_send = fdopen(dup(clnt_sock), "w");

    if (clnt_file == NULL || file_to_send == NULL)
        return ;

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

	// 검사 : 제대로 된 요청인가? (검사파트 다시보기)
	// 요청메시지가 http로 시작하는가? 메소드가 GET인가?
	if (strcmp(method, "GET") != 0 || \
		access(filename, F_OK) == -1 || \
		strstr(tmpline, "HTTP/") == NULL)
	{
		send_err(file_to_send);
		fclose(file_to_send);
		return ;
	}
	send_data(file_to_send, content, filename);
	close(clnt_sock);
}

void send_data(FILE *file_to_send, char *content, char *filename)
{
	FILE *html_file = fopen(filename, "r");

	char protocol[] = "HTTP/1.0 200 OK\r\n";
	char server[] = "Server: Linux Web Server\n";
	char cnt_len[] = "Content-length:2048\r\n";
	char cnt_type[100];
	sprintf(cnt_type, "Content-type:%s\r\n\r\n", content);

	fputs(protocol, file_to_send);
	fputs(server, file_to_send);
	fputs(cnt_len, file_to_send);
	fputs(cnt_type, file_to_send);

	char buffer[1024];
	while (fgets(buffer, 1024, html_file) != NULL)
	{
		fputs(buffer, file_to_send);
		fflush(file_to_send);
	}
	fclose(file_to_send);
	fclose(html_file);
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