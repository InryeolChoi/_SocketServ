#include "../../socket_header.h"
#include "../../socket_header.c"
#define BUF_SIZE 30

int main(int ac, char **av)
{
	if (ac != 3)
	{
		printf("usage : %s\n", av[0]);
		exit(0);
	}

	int sd;
	FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;

	fp = fopen("receive.dat", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(av[1]);
	serv_adr.sin_port = htons(atoi(av[2]));

	connect(sd, (struct sockaddr *)&serv_adr, sizeof (serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void *)buf, 1, read_cnt, fp);

	puts("Received file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
	return 0;
}