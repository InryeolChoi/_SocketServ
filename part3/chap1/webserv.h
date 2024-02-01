#ifndef WEBSERV_H
# define WEBSERV_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <pthread.h> 
# include <netdb.h>

# include <sys/time.h>
# include <sys/select.h>

# include <netinet/in.h>
# include <signal.h>
# include <fcntl.h>

# define BUF_SIZE 1024
# define SMALL_BUF 100

void *request_handler(void *arg);
void send_data(FILE *fp, char *ct, char *filename);
char *content_type(char *file);
void send_error(FILE *fp);
void error_handling(char *message);

#endif