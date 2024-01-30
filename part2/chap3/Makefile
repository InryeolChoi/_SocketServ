CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = kqserver

all : make

make : 
	$(CC) $(CFLAGS) main.cpp kqServer.cpp -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

.PHONY: all clean