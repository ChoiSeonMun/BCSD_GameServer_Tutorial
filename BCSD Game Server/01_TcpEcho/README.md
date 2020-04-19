## BCSD Lab Game Server Beginner Tutorial #1
### TCP Echo Server with Unicode

## 구현 시 고려사항
- 패킷의 크기는 고정 길이로 가정한다.
- 오류가 발생했을 시, 알맞은 시스템 오류를 같이 출력해야 한다.
> - [FormatMessage 참고](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage)
- backlog는 소켓의 최대 연결 가능 개수만큼 설정한다.
- 서버는 한 번에 하나의 클라이언트에게만 서비스를 제공할 수 있지만, 그 클라이언트와 연결이 끊기면 바로 다음 클라이언트에게 서비스가 제공되어야 한다.
- Unicode 기반으로 작성되어야 한다.