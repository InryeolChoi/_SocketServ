CC = gcc
CFLAGS = -Wall -Wextra -Wall

all: make

make:
	$(CC) $(CFLAGS) -o hostname gethostbyname.c
	$(CC) $(CFLAGS) -o hostaddr gethostbyaddr.c

clean :
	rm -rf hostname hostaddr

fclean : clean

.PHONY : all clean