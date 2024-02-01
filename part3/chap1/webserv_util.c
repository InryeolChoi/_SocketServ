#include "webserv.h"

void *request_handler(void *arg)
{
	int clnt_sock = *((int *)arg);	// 클라이언트 소켓
	char req_line[SMALL_BUF];		// 요청메시지를 담을 문자열
	FILE *clnt_read;				// 클라이언트 fd를 읽기 위한 파일
	FILE *clnt_write;				// 클라이언트에게 응답하기 위한 파일

	char method[10];
	char ct[15];
	char filename[30];

	clnt_read = fdopen(clnt_sock, "r");			// fd -> 읽기 전용 파일로
	clnt_write = fdopen(dup(clnt_sock), "w");	// fd -> 쓰기 전용 파일로
	
	// clnt_read 파일을 SMALL_BUF만큼 읽어 req_line에 옮긴다.
	fgets(req_line, SMALL_BUF, clnt_read);

	// req_line이 http 요청 문자열임을 알 수 있다. (req_line은 이미 맨 끝에 \n가 있음.)
	printf("request line : %s", req_line);

	// req_line에서 "HTTP/"를 찾는다. (= http에 의한 요청인지 확인)
	if (strstr(req_line, "HTTP/") == NULL)
	{
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return (void *)1;
	}

	// req_line에서 " /"를 기준으로 첫번째 토큰을 뽑고, 이를 method에 넣는다.
	strcpy(method, strtok(req_line, " /"));
	printf("method : %s\n", method);
	
	// req_line에서 " /"를 기준으로 두번째 토큰을 뽑고, 이를 filename에 넣는다.
	strcpy(filename, strtok(NULL, " /"));
	printf("filename : %s\n", filename);

	// content_type()을 통해 filename의 종류를 ct에 넣는다.
	strcpy(ct, content_type(filename));
	printf("ct : %s\n", ct);

	// get에 의한 요청인지 확인
	if (strcmp(method, "GET") != 0)
	{
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return (void *)1;
	}

	fclose(clnt_read);
	send_data(clnt_write, ct, filename);
	return (void *)0;
}

// 데이터 전송
void send_data(FILE *fp, char *ct, char *filename)
{
	char protocol[] = "HTTP/1.0 200 OK\r\n";
	char server[] = "Server: Linux Web Server\n";
	char cnt_len[] = "Content-length:2048\r\n";
	char cnt_type[SMALL_BUF];
	char buf[BUF_SIZE];
	FILE *send_file;

	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
	send_file = fopen(filename, "r");

	if (send_file == NULL)
	{
		send_error(fp);
		return ;
	}

	// 파일 fp에 http의 요소들을 담는다.
	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);

	// 요청한 html 파일(send_file)을 BUF_SIZE만큼 잘라서 buf에 담는다.
	while (fgets(buf, BUF_SIZE, send_file) != NULL)
	{
		// buf를 fp에 담아서 보내고, 이후 버퍼를 비운다.
		fputs(buf, fp);
		fflush(fp);
	}
	fflush(fp);
	fclose(fp);
}

// 타입 판단
char *content_type(char *file)
{
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];

	// file을 file_name에 복사
	strcpy(file_name, file);

	// '.'을 기준으로 file_name에서 첫번째 토큰 방출
	strtok(file_name, ".");

	// 두번째 토큰을 extension에 집어 넣음.
	strcpy(extension, strtok(NULL, "."));

	if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
		return "text/html";
	else
		return "text/plain";
}