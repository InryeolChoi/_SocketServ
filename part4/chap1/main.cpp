#include "chatserv.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage : " << av[0] << " <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		Chatserv cs(av[1], av[2]);
		cs.init_socket();
		cs.init_kqueue();

		cs.work_kqueue();
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}