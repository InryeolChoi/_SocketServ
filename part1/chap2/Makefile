CC = gcc
CFLAGS = -Wall -Wextra -Wall

all : tclient tserver

re : all

tclient: tcp_client.c
	$(CC) $(CFLAGS) -o hclient tcp_client.c

tserver : tcp_server.c
	$(CC) $(CFLAGS) -o hserver tcp_server.c

clean :
	rm -rf hclient hserver

fclean : clean

.PHONY : all clean