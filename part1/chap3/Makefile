CC = gcc
CFLAGS = -Wall -Wextra -Wall

all : make

re : all

make:
	$(CC) $(CFLAGS) -o endian_conv endian_conv.c
	$(CC) $(CFLAGS) -o inet_addr inet_addr.c
	$(CC) $(CFLAGS) -o inet_aton inet_aton.c
	$(CC) $(CFLAGS) -o inet_ntoa inet_ntoa.c

clean :
	rm -rf endian_conv inet_addr inet_aton inet_ntoa

fclean : clean

.PHONY : all clean