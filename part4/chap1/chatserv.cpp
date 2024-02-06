#include "chatserv.hpp"

Chatserv::Chatserv()
{}

Chatserv::Chatserv(const Chatserv &other)
{
	*this = other; 
}

Chatserv &Chatserv::operator=(const Chatserv &other)
{
	if (this != &other) {}
	return (*this);
}

Chatserv::Chatserv(std::string s1, std::string s2)
{
	std::istringstream prt_stream(s1);
	std::istringstream pw_stream(s2);

	prt_stream >> port;
	pw_stream >> password;

	if (prt_stream.fail() == true || pw_stream.fail() == true)
		throw Chatserv::NotNumberException();
}

Chatserv::~Chatserv()
{}

void Chatserv::init_socket()
{
	struct sockaddr_in serv_adr;
	int enable_reuse = 1;

	if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw Chatserv::WrongActionException();

	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(int)) < 0)
		throw Chatserv::WrongActionException();

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(this->port);

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		throw Chatserv::WrongActionException();
	if (listen(serv_sock, 5) == -1)
		throw Chatserv::WrongActionException();
}

void Chatserv::init_kqueue()
{
	if ((kq = kqueue()) == -1)
		throw Chatserv::WrongActionException();

	struct kevent serv_event;
    EV_SET(&serv_event, serv_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);

	if (kevent(kq, &serv_event, 1, NULL, 0, NULL) == -1)
		throw Chatserv::WrongActionException();
}

void Chatserv::work_kqueue()
{
	while (1)
	{
		struct kevent cur_events[MAX_EVENTS];
		int event_cnt = kevent(kq, NULL, 0, cur_events, MAX_EVENTS, NULL);
		if (event_cnt == -1)
			break ;

		printf("connected\n");
		for (int i = 0; i < event_cnt; i++)
		{
			int fd = cur_events[i].ident;
			if (fd == serv_sock)
				handle_connection();
			else
				handle_response(fd);
		}
	}
}

void Chatserv::handle_connection()
{
	struct sockaddr_in clnt_adr;
	socklen_t clnt_len = sizeof(clnt_adr);

	int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_len);
	if (clnt_sock == -1)
		throw Chatserv::WrongActionException();

	// 필터링 : 클라이언트 소켓에서 read를 잡도록 설정
	struct kevent clnt_event;
	EV_SET(&clnt_event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);

	// clnt_event에 걸리는 것 잡기
	if (kevent(kq, &clnt_event, 1, NULL, 0, NULL) == -1)
		throw Chatserv::WrongActionException();
}

void Chatserv::handle_response(int clnt_sock)
{
	char buffer[BUFFER_SIZE];

	while (1)
    {
		ssize_t bytesRead = recv(clnt_sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0)
			break ;
        buffer[bytesRead] = '\0';
        std::string line(buffer);
        std::cout << line << std::endl;
    }
	response_data(clnt_sock);
	close(clnt_sock);
}

void Chatserv::response_data(int clnt_sock)
{
	int clnt_send = dup(clnt_sock);
	std::string message;
	message += "1";

	send(clnt_send, message.c_str(), message.length(), 0);
}

const char *Chatserv::NotNumberException::what() const throw()
{
	return ("not a vaild port / password number");
}

const char *Chatserv::WrongActionException::what() const throw()
{
	return ("failed to operate / execute");
}