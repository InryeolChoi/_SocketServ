#include "webserv.h"
#define MAX_REQUEST_SIZE 1024

void response_to(int clnt_sock)
{
	char request_line[1024];
    FILE *clnt_file = fdopen(clnt_sock, "r");
    FILE *file_to_send = fdopen(dup(clnt_sock), "w");

    if (clnt_file == NULL || file_to_send == NULL) {
        perror("fdopen failed");
        free(request_line);
        return ;
    }

	fgets(request_line, 1024, clnt_file);	// 문제
	printf("%s\n", request_line);

	// 파싱 : 소켓 속 html 요청메시지
	char method[1024];
	char filename[1024];
	char content[1024];

	printf("11111111111\n");
	strcpy(method, strtok(request_line, " /"));
	strcpy(filename, strtok(NULL, " /"));
	strcpy(content, get_content_name(filename));

	// 검사 : 제대로 된 요청인가?
	// 요청메시지가 http로 시작하는가? 메소드가 GET인가?
	if (strstr(request_line, "HTTP/") == NULL || \
		strcmp(method, "GET") != 0 || \
		access(filename, F_OK) == -1)
	{
		send_err(file_to_send);
		fclose(file_to_send);
		return ;
	}
	// 맞으면 보낸다.
	send_data(file_to_send, content, filename);
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
	fflush(file_to_send);
	fclose(file_to_send);
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