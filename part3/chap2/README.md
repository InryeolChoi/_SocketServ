# 실전 http 서버 만들기 (2)
> c언어, html, kqueue(), 하나의 포트
> 단, Mac에서만 실행 가능.

```
1. 터미널 창에 make라고 친다.
2. 터미널 창에서 ./server 9190 이라고 친다.
3. http://localhost:9190/index.html 로 접속 -> 올바른 페이지
	TCP/IP Socket Programming is fun! 이라고 나옴
4. 기타 주소 -> 에러 페이지
	400 Bad request. Please type correct URL 이라고 나옴.
```

## 구조
webserv.c에 있는 main() 기준
1. 터미널로 포트번호를 받아들인다.
2. 서버소켓와 kq를 등록한다.
	- 서버소켓 등록 함수 : init_serv_sock()
	- kq 등록 함수 : init_kqueue()
3. 처음에는 EV_SET()을 이용해 서버가 read을 잡도록 설정
	- 서버소켓을 kq에 등록시키는 것이라고 생각.
	- catch_read_kqueue()
4. 서버 굴리면서 요청 -> 응답 보내기
	- work_kqueue()

work_kqueue() 구조
> 전부 while(1)로 계속 돌아간다고 생각하면 됨.
1. kevent()를 통해 kq로 들어오는 이벤트를 cur_events 배열에 넣는다.
2. kevent()의 반환값 = 이벤트의 갯수
	- 만약 그 값이 -1이면 break
3. for문 시작
	1. 클라이언트 소켓을 만든다.
	2. i번째 cur_events의 fd가 소켓과 같은 경우
		1. 클라이언트 소켓을 만든다.
		2. 이벤트를 담을 clnt_event를 만든다.
		3. EV_SET()을 이용해 클라이언트 소켓에서 read를 잡는다.
		4. 이를 clnt_event에 담는다.
		5. clnt_event를 kq에 등록.
	3. i번째 cur_events의 fd가 소켓이 아닌 경우 -> response_to()
		1. 해당 fd를 클라이언트 소켓이라고 생각.
		2. fd를 열어 요청읽기용 파일과 응답용 파일을 만든다.
		3. 요청읽기용 파일을 파싱 + 검사 (http, 메소드 검사)
		4. 맞으면 send_err(응답용 파일) -> 에러페이지 보내기.
		5. 아니면 send_data(응답용 파일, 요청파일_주소) -> 정보페이지 보내기.
		6. 다 끝나면 반드시 클라이언트 소켓 + 파일들을 닫는다.  

send_data(응답용 파일, 요청파일_주소)
1. 프로토콜, 서버, content_length, content_type를 적는다.
2. 응답용 파일을 이용해 위의 4가지를 먼저 보낸다.
3. 요청파일_주소을 열어 요청파일을 만든다.
4. 요청파일을 읽어가며 일정 사이즈만큼 버퍼에 넣는다.
5. 버퍼를 응답용 파일에 넣는다. (알아서 보내짐.)


## 깨달은것
- C언어인만큼 할당을 제대로 해야 한다.
- 소켓과 그 소켓을 담은 file로 이뤄져 있는데, 보내고 난 뒤 반드시  
	파일과 소켓을 모두 닫아줘야 한다.
