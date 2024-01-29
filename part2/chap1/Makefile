CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: make

make :
	$(CC) $(CFLAGS) -o select select.c
	$(CC) $(CFLAGS) -o client echo_client.c
	$(CC) $(CFLAGS) -o selserv echo_selectserv.c

clean:
	rm -rf select
	rm -rf client selserv

fclean: clean

.PHONY: all clean
 