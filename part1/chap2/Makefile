CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: tclient

tclient: tcp_client.c
	$(CC) $(CFLAGS) -o hclient tcp_client.c

clean :
	rm -rf hclient

fclean : clean

.PHONY : all clean