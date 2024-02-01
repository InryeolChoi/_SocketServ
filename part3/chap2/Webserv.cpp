#include "webserv_linux.h"

void Webserv::serv_init()
{
	serv_sock = socket();

	memset();
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.
}
