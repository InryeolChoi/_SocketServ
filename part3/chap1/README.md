# 기초 서버 설계
## 작동방식
1. 터미널에 make를 친다.
2. ./server 9190  
- 9190 대신 다른 포트번호를 쓰는 것도 가능하다.
- 단, 잘 알려진 포트번호는 피해야 한다.
3. http://localhost:9190/index.html

## 원리
```
1. 요청을 받는다.
2. 
3. 
```

## 코드 구조
webserv.c  
wevserv_util.c -> 유틸
webserv_err.c -> 에러 