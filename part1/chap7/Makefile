CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: make

make:
	$(CC) $(CFLAGS) -o fserver file_server.c
	$(CC) $(CFLAGS) -o fclient file_client.c

clean :
	rm -rf fserver fclient receive.dat

fclean : clean

.PHONY : all clean