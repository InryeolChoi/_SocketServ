#include "webserv.h"

void response_err(FILE *send_file)
{
	char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
	char server[] = "Server: Linux Web Server \r\n";
	char cnt_len[] = "Content-length:2048 \r\n";
	char cnt_type[] = "Content-type:text/html\r\n\r\n";
	char content[] = "<html><head><title>NETWORK</title></head>"
					"<body><font size=+5><br>400 Bad request. Please type correct URL"
					"</font></body></html>";

	fputs(protocol, send_file);
	fputs(server, send_file);
	fputs(cnt_len, send_file);
	fputs(cnt_type, send_file);
	fputs(content, send_file);

	fflush(send_file);
}

void response_data(FILE *file_to_send, char *content, char *filename)
{
	char protocol[] = "HTTP/1.0 200 OK\r\n";
	char server[] = "Server: Linux Web Server\n";
	char cnt_len[] = "Content-length:2048\r\n";
	char cnt_type[100];
	sprintf(cnt_type, "Content-type:%s\r\n\r\n", content);

	fputs(protocol, file_to_send);
	fputs(server, file_to_send);
	fputs(cnt_len, file_to_send);
	fputs(cnt_type, file_to_send);

	FILE *html_file = fopen(filename, "r");
	char buffer[1024];
	while (fgets(buffer, 1024, html_file) != NULL)
	{
		fputs(buffer, file_to_send);
		fflush(file_to_send);
	}
	fclose(html_file);
}
