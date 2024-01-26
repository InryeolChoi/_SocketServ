# chap5

## 구동방법
```
make op : op_server op_client 생성
make echo : echo_server echo_client 생성
```

## 1. 에코 서버 제작
> 앞의 4단원에서 구현한 서버의 문제점은 다음과 같았다.

```
client의 write() → server의 read()로 가는 과정에서 문자열의 일부만 읽힐 수 있다.
server의 write() → client의 read() 과정에서도 문자열의 일부만 전해진다.
그래서 문자열이 짤려서 client에게 올 수 있다.
```

따라서 while()을 이용해서 


## 2. 계산기 서버 제작
> 계산기 기능을 가진 서버를 구현해보자.  
이번에는 client가 write()하는 길이가 어디인지 확실히 알 수 없다.

