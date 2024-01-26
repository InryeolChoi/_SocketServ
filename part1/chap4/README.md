# chap4
* iterative 서버 만들기
- 여러 클라이언트의 요청을 받을 수 있도록 바꾼 모습
	- 그래서 iterative 서버라는 것.
	- 계속 요청을 넣으면 계속 수락.
- 반복문을 삽입해서 accept 함수를 반복.

## 구조
```
1. echo_server.c를 이용해서 eserver 만들고 구동
2. echo_client.c를 이용해서 eclient1 ~ eclient5개 만들기
3. eserver는 먼저 받는 순서대로 계속 요청 처리
	eclient1, eclient2, .. 이런 식으로 들어간다 치면...
	a. eclient1의 요청 무한처리
	b. eclient2의 요청 무한처리
	c. ...
```

## 문제점 in eclient
```
write(sock, message, strlen(message));
str_len = read(sock, message, BUF_SIZE - 1);
message[str_len] = 0;
printf("message from server : %s", message);
```
