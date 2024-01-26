CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: eserver eclient1 eclient2 eclient3 eclient4 eclient5

re: all

eserver: echo_server.c
	$(CC) $(CFLAGS) -o eserver echo_server.c

eclient1: echo_client.c
	$(CC) $(CFLAGS) -o eclient1 echo_client.c

eclient2: echo_client.c
	$(CC) $(CFLAGS) -o eclient2 echo_client.c

eclient3: echo_client.c
	$(CC) $(CFLAGS) -o eclient3 echo_client.c

eclient4: echo_client.c
	$(CC) $(CFLAGS) -o eclient4 echo_client.c

eclient5: echo_client.c
	$(CC) $(CFLAGS) -o eclient5 echo_client.c

clean:
	rm -rf eserver eclient1 eclient2 eclient3 eclient4 eclient5

fclean: clean

.PHONY: all clean
