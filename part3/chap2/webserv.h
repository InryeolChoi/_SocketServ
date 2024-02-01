#ifndef WEBSERV_H
#define WEBSERV_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAX_EVENTS 10
#define SMALL_BUF 100

void 	work_kqueue(int kq, int serv_sock);
void 	catch_read_kqueue(int kq, int serv_sock);

void 	error_handling(char *message);
int 	init_serv_sock(char *port);
int		init_kqueue(int serv_sock);

int 	add_client(int kq, int serv_sock);
void	response_to(int clnt_sock);
char 	*get_content_name(char *file);
void 	send_err(FILE *send_file);
void 	send_data(FILE *file_to_send, char *content, char *filename);

#endif