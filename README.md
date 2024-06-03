# IRC : IRC 서버 구현 프로젝트
<img width="721" alt="image" src="https://github.com/im-madylin/IRC/assets/85945788/bf47116f-4335-44f5-a336-18972208d905">


### IRC란,
> "IRC(Internet Relay Chat)는 1988년에 만들어진 인터넷에 연결된 컴퓨터 간의 실시간 문자 메시징을 위한 프로토콜 입니다 . 두 사용자 간의 개인 메시지, 데이터 전송을 지원하지만 주로 "channels"라는 채팅방에서 그룹 토론에 사용됩니다. 다양한 서버 측 및 클라이언트 측 명령을 제공합니다. 2011년 4월 기준으로 상위 100개 IRC 네트워크는 수십만 개의 채널에서 동시에 500,000명 이상의 사용자에게 서비스를 제공했습니다.</br>
> [출처](https://www.radware.com/security/ddos-knowledge-center/ddospedia/irc-internet-relay-chat/)

## 프로젝트 요구사항
- C++ 98로 IRC 서버를 개발해야 합니다.
- 서버는 동시에 여러 클라이언트를 처리할 수 있어야 하며 중단되지 않아야 합니다.
- 이러한 모든 작업(읽기, 쓰기 및 수신 등)을 처리하는 데는 하나의 poll()(또는 이와 동등한 것)만 사용할 수 있습니다.
- IRC 클라이언트는 오류 없이 서버에 연결할 수 있어야 합니다.
- 클라이언트와 서버 간의 통신은 TCP/IP(v4)를 통해 이루어져야 합니다.
- 우리 서버에서 IRC 클라이언트를 사용하는 것은 공식 IRC 서버에서 사용하는 것과 유사해야 합니다. 그러나 다음 기능만 구현하면 됩니다.
  - IRC 클라이언트를 사용하여 인증하고, 닉네임/사용자 이름을 설정하고, 채널에 가입하고, 비공개 메시지를 보내고 받을 수 있어야 합니다.
  - 한 클라이언트에서 채널로 전송된 모든 메시지는 채널에 참여한 다른 모든 클라이언트에게 전달되어야 합니다.
  - 운영자와 일반 사용자가 있어야 합니다.
  - 그런 다음 운영자에게 특정한 명령을 구현해야 합니다.
- 오늘의 운세를 알려주는 Bot을 구현해야 합니다.


## 설치 방법
1. 저장소 복제
   ```
   git clone https://github.com/im-madylin/ft_irc.git
   ```
2. 디렉토리 이동
   ```
   cd ft_irc
   ```
3. 컴파일
   ```
   make
   ```
## 연결 방법
- IRC 서버
  ```
  ./ircserv <PORT> <PASSWORD>
  ```
  - PORT : IRC 서버가 연결을 수신할 port
  - PASSWORD : IRC 클라이언트가 IRC 서버 연결 시 사용할 password  
- IRC 클라이언트 (LimeChat, WeeChat 등 IRC 클라이언트를 사용할 수도 있습니다.)
  ```
  nc <IP ADDRESS> <PORT>
  ```
  - IP ADDRESS : 호스트 IP 주소
  - PORT : 연결할 IRC 서버 port

## 구현한 명령어
IRC 서버에서 사용할 수 있는 명령은 다음과 같습니다.
| 명령어 | 설명 |
|--|--|
| PASS | 연결할 서버의 비밀번호를 입력합니다. <br /> ``` PASS <PASSWORD> ``` |
| NICK | 사용자에게 닉네임을 설정하거나 기존 닉네임을 변경합니다.  <br /> ``` NICK <NICKNAME> ``` |
| USER | 연결 시작 시 새 사용자의 사용자 이름과 실제 이름을 설정합니다.  <br /> ``` USER <USERNAME> 0 * <REALNAME> ``` |
| PING | 서버와의 연결을 확인합니다. <br /> ``` PING ``` |
| MODE | 설정을 변경합니다.(Operator만 사용 가능) <br /> ``` MODE <CHANNEL> <MODE> ``` <br /> \<MODE> : +/- 를 사용하여 설정 추가/삭제 <br /> i : 초대한 사람만 입장 가능, t : Operator만 Topic 변경 가능, k \<PASSWORD> : 채널에 비밀번호 설정, o \<NICKNAME> : Operator 권한 부여, l \<NUMBER> : NUMBER만큼 입장 가능  |
| JOIN | 채널에 참여를 요청합니다. <br /> ``` JOIN <CHANNEL>{,<CHANNEL>} [<KEY>{,<KEY>}] ```  |
| INVITE | 채널에 사용자를 초대합니다. <br /> ``` INVITE <CHANNEL> ``` |
| PART | 채널에서 퇴장합니다. <br /> ``` PART <CHANNEL> ``` |
| TOPIC | 채널의 주제를 변경하거나 확인합니다. <br /> ``` TOPIC <CHANNEL> {<CONTENTS>} ```  |
| KICK | 채널에서 사용자를 퇴장시킵니다.  <br /> ``` KICK <CHANNEL> ``` |
| PRIVMSG | 사용자나 채널에 메시지를 전송합니다. <br /> ``` PRIVMSG <NICKNAME/CHANNEL> <MESSAGE> ``` |
| BOT | 오늘의 운세 봇을 실행하여 운세를 받습니다.  <br /> ``` PRIVMSG <CHANNEL> !<BIRTHDAY> ```  |
| NOTICE | 사용자에게 메시지를 전송합니다. <br /> ``` NOTICE <NICKNAME> <MESSAGE> ``` |
| WHO | 채널 참여자 정보를 확인합니다.  <br /> ``` WHO <CHANNEL> ``` |
| QUIT | 클라이언트 세션을 종료합니다.  <br /> ``` QUIT ```|
