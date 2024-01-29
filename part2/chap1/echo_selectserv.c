#include "../../socket_header.h"
#include "../../socket_header.c"
#define BUF_SIZE2 100

int main(int ac, char **av)
{
	int serv_sock, clnt_sock, res;
	struct sockaddr_in serv_adr, clnt_adr;

	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_size;
	int fd_max, str_len, fd_num;
	char buf[BUF_SIZE2];

	if (ac != 2)
	{
		printf("usage : %s\n", av[0]);
		exit(0);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	res = bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
	if (res == -1)
		error_handling("bind() error");
	res = listen(serv_sock, 5);
	if (res == -1)
		error_handling("listen() error");
	
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while (1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break ;
		if (fd_num == 0)
			continue ;
		
		for (int i = 0; i < fd_max + 1; i++)
		{
			if (FD_ISSET(i, &cpy_reads))
			{
				if (i == serv_sock)
				{
					adr_size = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_size);
					FD_SET(clnt_sock, &reads);

					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					
					printf("connected client : %d\n", clnt_sock);
				}
				else // read message
				{
					str_len = read(i, buf, BUF_SIZE2);
					if (str_len == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client : %d\n", i);
					}
					else
						write(i, buf, str_len);
				}
			}
		}
	}

	close(serv_sock);
	return 0;
}