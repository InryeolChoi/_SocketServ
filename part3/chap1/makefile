CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = server

all : make

make : 
	$(CC) $(CFLAGS) webserv_*.c webserv.c -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

.PHONY: all clean