#include "../../socket_header.h"
#define BUF_SIZE2 30

int main()
{
	fd_set reads, temps;		// fd를 담는 구조체
	int result, str_len;
	char buf[BUF_SIZE2];
	struct timeval timeout; 	// 시간초과를 설정

	FD_ZERO(&reads);			// reads 속 비트들을 초기화
	FD_SET(0, &reads);			// 0번 fd를 reads안에 넣음

	while (1)
	{
		temps = reads;					// 변수 초기화
		timeout.tv_sec = 5;				// 매번 새로 초기화를 해야한다.
		timeout.tv_usec = 0;			// 이래야 타임아웃 설정까지 시간을 제대로 설정.
		result = select(1, &temps, 0, 0, &timeout); // select 사용

		if (result == -1)				// select 함수가 0을 반환
		{
			puts("select() error!");
			break ;
		}
		else if (result == 0)			// select 함수가 0을 반환
			puts("Time-out");
		else							// select 함수가 0보다 큰 수를 반환 (fd가 맞는지 보기)
		{
			if (FD_ISSET(0, &temps))	// fd가 표준입력(0)이 맞는지 확인 후 출력
			{
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}

	return 0;
}