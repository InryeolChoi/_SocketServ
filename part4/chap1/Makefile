CC = g++
CFLAGS = -Wall -Wextra -Werror
NAME = chatserv
FILE = chatserv.cpp main.cpp

all : make

re : fclean all

make : 
	$(CC) $(CFLAGS) $(FILE) -o $(NAME)

clean:
	rm -rf $(NAME)
	rm -rf *.dSYM

fclean: clean

.PHONY: all clean