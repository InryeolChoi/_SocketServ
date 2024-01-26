CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: echo op

echo:
	$(CC) $(CFLAGS) -o echo_server2 echo_server2.c
	$(CC) $(CFLAGS) -o echo_client2 echo_client2.c

op:
	$(CC) $(CFLAGS) -o opserver op_server.c
	$(CC) $(CFLAGS) -o opclient op_client.c

clean :
	rm -rf echo_server2 echo_client2 opserver opclient

fclean : clean

.PHONY : all clean