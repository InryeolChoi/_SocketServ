CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: make

make :
	$(CC) $(CFLAGS) -o client echo_client.c
	$(CC) $(CFLAGS) -o epserv echo_epollserv.c

clean:
	rm -rf client epserv

fclean: clean

.PHONY: all clean