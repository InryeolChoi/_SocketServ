# chap2
## 구동방법

```
./hserver (포트번호)
./hclient 127.0.0.1 (포트번호)
```
포트번호는 919x를 추천  
ip주소 127.0.0.1은 루프백 주소라고 하며, 자기 자신을 의미

## 정상적인 결과
```
message from server: hello world
function read call count : 12
```
## 참고사항
이 서버는 같은 포트에서 바로 재실행이 불가능하다.
