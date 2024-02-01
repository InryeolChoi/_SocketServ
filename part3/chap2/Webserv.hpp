#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/time.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

class webserv 
{
	public:
		int serv_sock, clnt_sock;
		struct sockaddr_in serv_adr, clnt_adr;
		int clnt_adr_size;
		char buf[BUF_SIZE];

		void serv_init();
		
};