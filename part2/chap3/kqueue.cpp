#include <sys/event.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av)
{
	struct kevent event;	// 지켜보고 싶은 이벤트
	struct kevent tevent;	// 일어난(triggered) 모든 이벤트
	int kq, fd, ret;

	if (ac != 2)
		err(EXIT_FAILURE, "Usage : %s <path>\n", av[0]);
	fd = open(av[1], O_RDONLY);

	// kqueue 만들기
	kq = kqueue();
	if (kq == -1)
		err(EXIT_FAILURE, "kqueue() failed");

	// kevent 초기화
	EV_SET(&event, fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_WRITE, 0, NULL);

	ret = kevent(kq, &event, 1, NULL, 0, NULL);
	if (ret == -1)
		err(EXIT_FAILURE, "kevent register");
	if (event.flags & EV_ERROR)
		errx(EXIT_FAILURE, "event error : %s", strerror(event.data));

	for (;;)
	{
		ret = kevent(kq, NULL, 0, &tevent, 1, NULL);
		if (ret == -1)
			err(EXIT_FAILURE, "kevent wait");
		else if (ret > 0)
			printf("something was written : %s\n", av[1]);
	}
}