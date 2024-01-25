CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: hclient hserver

hclient: hello_client.c
	$(CC) $(CFLAGS) -o hclient hello_client.c

hserver: hello_server.c
	$(CC) $(CFLAGS) -o hserver hello_server.c

clean :
	rm -f hclient hserver *.o

fclean : clean

.PHONY : all clean