#include "echo.hpp"
using namespace std;

void exit_with_perror(const string& msg)
{
    cerr << msg << endl;
    exit(EXIT_FAILURE);
}

void change_events(vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

	// change_list 안에 이벤트를 집어넣는다.
    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

void disconnect_client(int client_fd, map<int, string>& clients)
{
    cout << "client disconnected: " << client_fd << endl;
    close(client_fd);
    clients.erase(client_fd);
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : %s <port> \n", av[0]);
		exit(1);
	}

    // 소켓 초기화
    int serv_sock;
    struct sockaddr_in server_addr;

    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        exit_with_perror("socket() error\n" + string(strerror(errno)));

	// 포트 및 ip설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(av[1]));

	// bind()와 listen()
    if (bind(serv_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        exit_with_perror("bind() error\n" + string(strerror(errno)));
    if (listen(serv_sock, 5) == -1)
        exit_with_perror("listen() error\n" + string(strerror(errno)));

	// 파일 상태를 설정(F_SETFL), 논블록으로(O_NONBLOCK)
    fcntl(serv_sock, F_SETFL, O_NONBLOCK);

	// kqueue 만들어주기
    int kq; // kqueue 인스턴스의 fd.  
    if ((kq = kqueue()) == -1)
        exit_with_perror("kqueue() error\n" + string(strerror(errno)));

	map<int, string> clients; 			// 클라이언트 소켓과 데이터를 맵으로 저장
    vector<struct kevent> change_list;	// kevent를 저장하는 벡터
    struct kevent event_list[8];		// kevent()로 발생한 이벤트를 저장하는 배열
    struct kevent temp_event;			// 임시로 사용될 struct kevent 구조체

	// EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    EV_SET(&temp_event, serv_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	// temp_event 구조체를 초기화.
	// 서버 소켓(serv_sock)에 대한 읽기 이벤트(EVFILT_READ)를 감시
	// 이벤트를 추가하고 활성화도 함(EV_ADD | EV_ENABLE)
	// 나머지는 그냥 전부 0.

	// 인제 이벤트를 kqueue에 등록되어 서버 소켓에서 읽기 이벤트를 감시.
	change_list.push_back(temp_event); // temp_event를 집어 넣음.
	cout << "echo server started" << endl;

    int new_events;
    struct kevent* curr_event;

    while (1)
    {
		// 몇 개의 이벤트가 일어났는지 확인
        new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
		if (new_events == -1)
            exit_with_perror("kevent() error\n" + string(strerror(errno)));
		// change_list = event_list에 담을 이벤트의 종류 = 일종의 거름망.
		// 이벤트를 기다리고 있다가, kq로 들어온(= 발생한) 이벤트가 change_list에 있으면, event_list에 담기게 된다.

		// change_list를 초기화
        change_list.clear();

		// event_list를 돌려가며 지금의 fd가 무엇인지 판단 
		for (int i = 0; i < new_events; i++)
		{
			curr_event = &event_list[i];

			// 에러를 잡아내는 부분
            if (curr_event->flags & EV_ERROR)
            {
				// 만약 현재 이벤트의 fd가 server_socket이면
                if (curr_event->ident == (uintptr_t)serv_sock)
                    exit_with_perror("server socket error");
				// 아니라면
                else
                {
                    cerr << "client socket error" << endl;
                    disconnect_client(curr_event->ident, clients);
                }
            }

			// 현재 이벤트 == "읽기" 이벤트일 때
            else if (curr_event->filter == EVFILT_READ)
            {
				// 만약 현재 이벤트의 fd가 server_socket이면
                if (curr_event->ident == (uintptr_t)serv_sock)
                {
					// 클라이언트 소켓을 만든다.
                    int clnt_sock;
                    if ((clnt_sock = accept(serv_sock, NULL, NULL)) == -1)
                        exit_with_perror("accept() error\n" + string(strerror(errno)));
					cout << "accept new client: " << clnt_sock << endl;
					fcntl(clnt_sock, F_SETFL, O_NONBLOCK);

					// 클라이언트 소켓(clnt_sock)에 대한 읽기 이벤트(EVFILT_READ) / 쓰기 이벤트(EVFILT_WRITE)를 감시
                    change_events(change_list, clnt_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    change_events(change_list, clnt_sock, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    clients[clnt_sock] = "";
                }

				// 지금 이벤트가 clients의 끝이 아니라면
                else if (clients.find(curr_event->ident)!= clients.end())
                {
					// 클라이언트에서 데이터를 읽는다.
                    char buf[1024];
                    int n = read(curr_event->ident, buf, sizeof(buf));

					// 데이터가 한줄도 없으면
                    if (n <= 0)
                    {
                        if (n < 0)
							cerr << "client read error!" << endl;
						// 해당 이벤트를 지운다.
                        disconnect_client(curr_event->ident, clients);
                    }

					// 데이터가 있으면 출력
                    else
                    {
                        buf[n] = '\0';
                        clients[curr_event->ident] += buf;
                        cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << endl;
                    }
                }
            }

			// 현재 이벤트 == "출력" 이벤트일 때 (소켓이 클라이언트 소켓인 경우)
            else if (curr_event->filter == EVFILT_WRITE)
            {
                map<int, string>::iterator it = clients.find(curr_event->ident);
                if (it != clients.end())
                {
                    if (clients[curr_event->ident] != "")
                    {
                        int n;
                        if ((n = write(curr_event->ident, clients[curr_event->ident].c_str(),
                                        clients[curr_event->ident].size()) == -1))
                        {
                            cerr << "client write error!" << endl;
                            disconnect_client(curr_event->ident, clients);  
                        }
                        else
                            clients[curr_event->ident].clear();
                    }
                }
            }


		}
	}
	return 0;
}