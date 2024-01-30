CC = g++
CFLAGS = -Wall -Wextra -Werror

all : make

make : 
	$(CC) $(CFLAGS) echo_client.cpp -o eclient
	$(CC) $(CFLAGS) echo_kqserv.cpp -o eserver

clean:
	rm -rf eclient eserver

fclean: clean

.PHONY: all clean