#pragma once
#ifndef SOCKET_HEADER_H
# define SOCKET_HEADER_H

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

void error_handling(char *message);

#endif