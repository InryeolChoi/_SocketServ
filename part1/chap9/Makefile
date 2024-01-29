CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: make

make :
	$(CC) $(CFLAGS) -o eclient echo_client.c
	$(CC) $(CFLAGS) -o eserver reuseadr_eserver.c
	$(CC) $(CFLAGS) -o getbuf get_buf.c
	$(CC) $(CFLAGS) -o setbuf set_buf.c

clean:
	rm -rf eclient eserver getbuf setbuf

fclean: clean

.PHONY: all clean
