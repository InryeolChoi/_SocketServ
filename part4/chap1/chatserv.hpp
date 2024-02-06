#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024

class Chatserv
{
	private:
		int port;
		int password;
		int serv_sock;
		int kq;

		Chatserv();
		Chatserv(const Chatserv &other);
		Chatserv &operator=(const Chatserv &other);

	public:
		Chatserv(std::string s1, std::string s2);
		~Chatserv();

		void init_socket();
		void init_kqueue();
		void work_kqueue();
		void handle_connection();
		void handle_response(int clnt_sock);
		void response_data(int clnt_sock);

		class NotNumberException : public std::exception
		{
			public:
				virtual const char *what() const throw();	
		};
		class WrongActionException : public std::exception
		{
			public:
				virtual const char *what() const throw();	
		};
};