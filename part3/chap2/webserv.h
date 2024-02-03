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
#define MAX_REQUEST_SIZE 1024

void 	error_handling(char *message);

// webserv_init.c
int 	init_serv_sock(char *port);
int		init_kqueue(int serv_sock);
void 	catch_read_kqueue(int kq, int serv_sock);

// webserv_kque.c
void 	work_kqueue(int kq, int serv_sock);
void 	handle_clnt_connection(int kq, int serv_sock);
void	handle_clnt_response(int clnt_sock);
char 	*get_content_name(char *file);

// webserv_resp.c
void 	response_err(FILE *send_file);
void 	response_data(FILE *file_to_send, char *content, char *filename);

#endif