CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = server

all : make

re : fclean all

make : 
	$(CC) $(CFLAGS) webserv_*.c webserv.c -o $(NAME)

clean:
	rm -rf $(NAME)
	rm -rf *.dSYM

fclean: clean

.PHONY: all clean