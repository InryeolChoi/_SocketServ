CC = gcc
CFLAGS = -Wall -Wextra -Wall

all : make

re : all

make:
	$(CC) $(CFLAGS) -o echo_server echo_server.c
	$(CC) $(CFLAGS) -o echo_client echo_client.c

clean :
	rm -rf echo_server echo_client

fclean : clean

.PHONY : all clean