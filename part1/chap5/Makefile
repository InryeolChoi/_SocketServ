CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: echo op

echo:
	$(CC) $(CFLAGS) -o echo_server2 echo_server2.c
	$(CC) $(CFLAGS) -o echo_client2 echo_client2.c

op:
	$(CC) $(CFLAGS) -o op_server op_server.c
	$(CC) $(CFLAGS) -o op_client op_client.c

clean :
	rm -rf echo_server2 echo_client2 op_server op_client

fclean : clean

.PHONY : all clean