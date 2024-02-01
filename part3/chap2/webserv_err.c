#include "webserv.h"

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void send_err(FILE *send_file)
{
	char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
	char server[] = "Server: Linux Web Server \r\n";
	char cnt_len[] = "Content-length:2048 \r\n";
	char cnt_type[] = "Content-type:text/html\r\n\r\n";
	char content[] = "<html><head><title>NETWORK</title></head>"
					"<body><font size=+5><br>The error has occured!"
					"</font></body></html>";

	fputs(protocol, send_file);
	fputs(server, send_file);
	fputs(cnt_len, send_file);
	fputs(cnt_type, send_file);
	(void) content;

	fflush(send_file);
}