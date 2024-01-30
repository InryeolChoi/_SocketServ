CC = g++
CFLAGS = -Wall -Wextra -Werror

all : make

make : 
	$(CC) $(CFLAGS) echo_client.cpp -o eclient
	$(CC) $(CFLAGS) echo_kqserv.cpp -o eserver
	$(CC) $(CFLAGS) kqueue.cpp -o kqueue
	@touch new.txt

clean:
	rm -rf kqueue new.txt
	rm -rf eclient eserver

fclean: clean

.PHONY: all clean