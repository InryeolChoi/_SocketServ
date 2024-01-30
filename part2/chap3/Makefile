CC = g++
CFLAGS = -Wall -Wextra -Werror

all : make

make : 
	$(CC) $(CFLAGS) kqueue.cpp -o kqueue
	@touch new.txt

clean:
	rm -rf kqueue new.txt

fclean: clean

.PHONY: all clean