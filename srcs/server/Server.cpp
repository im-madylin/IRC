#include "Server.hpp"
#include "../user/User.hpp"
#include "../command/Command.hpp"

Server::Server(string port, string password) 
: _port(atoi(port.c_str())), _password(password)
{
	this->_serverName = SERVER_NAME;
	this->_command = new Command(this);
	initServer();
}

Server::~Server()
{
	this->_users.clear();
	this->_channels.clear();
}

int Server::getPort() const
{
	return this->_port;
}

string Server::getServerName() const
{
	return this->_serverName;
}

string Server::getPassword() const
{
	return this->_password;
}

void Server::setServerName(string serverName)
{
	this->_serverName = serverName;
}

void Server::run()
{
	initKqueue();
	while (1)
	{
		struct timespec timeout = {KQUEUE_TIMEOUT, 0}; // 3초 타임아웃

		// kqueue에 등록된 이벤트가 발생할 때까지 대기, 이벤트 수만큼 반환
		int eventCount = kevent(this->_kq, NULL, 0, this->_events, KQUEUE_SIZE, &timeout);
		if (eventCount == -1)
		{
			cerr << "kevent() error" << endl;
			exit(1);
		}

		// 발생한 이벤트 처리
		for (int i = 0; i < eventCount; i++)
			handleEvent(_events[i]);
	}
}

void Server::initServer()
{
	struct sockaddr_in serverAddr;

	// server 소켓 생성 AF_INET: IPv4, SOCK_STREAM: TCP
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocket == -1)
	{
		cerr << "socket() error" << endl;
		exit(1);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// 소켓 옵션 설정, SO_REUSEADDR: 커널이 소켓을 사용하는 중에도 포트를 사용할 수 있게 해줌
	int optval = 1;
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
	{
		cerr << "setsockopt failed" << endl;
		exit(1);
	}

	// non-blocking 모드로 변경
	fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);

	// 해당 주소와 server로 들어오는 클라이언트의 연결을 수락할 수 있도록 함
	if (::bind(this->_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		cerr << "bind() error" << endl;
		exit(1);
	}
	
	// 연결 요청 대기열 생성
	if (listen(this->_serverSocket, serverAddr.sin_port) == -1)
	{
		cerr << "listen() error" << endl;
		exit(1);
	}
}

void Server::initKqueue()
{
	// kqueue 생성
	if ((this->_kq = kqueue()) == -1)
	{
		cerr << "kqueue() error" << endl;
		exit(1);
	}

	// 이벤트 설정 -> 읽기 이벤트 감지, 이벤트 추가, 이벤트 활성화
	EV_SET(&this->_change, this->_serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	// kqueue에 이벤트 등록	
	if (kevent(this->_kq, &this->_change, 1, NULL, 0, NULL) == -1)
	{
		cerr << "kevent() error" << endl;
		exit(1);
	}
}

// client 연결 수락 및 소켓 생성
void Server::acceptConnection()
{
	// client 소켓 정보 저장할 구조체
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	User *user;

	// 클라이언트 소켓 생성 및 연결
	memset(&clientAddr, 0, sizeof(clientAddr));
	int clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
	if (clientSocket == -1)
	{
		cerr << "accept() error" << endl;
		exit(1);
	}
	// non-blocking 모드로 변경
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);

	// kqueue에 등록하여 client 소켓을 감시
	EV_SET(&this->_change, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	// EV_SET(&this->_change, clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(this->_kq, &this->_change, 1, NULL, 0, NULL) == -1)
	{
		cerr << "kevent() error" << endl;
		exit(1);
	}
	cout << "new connection from " << clientSocket << endl;

	// user 객체 생성 및 서버에 저장
	user = new User(clientSocket, inet_ntoa(clientAddr.sin_addr));
	_users.insert(make_pair(clientSocket, user));
}

void Server::disconnetClient(int clientSocket)
{
	map<int, User *>::iterator it = this->_users.find(clientSocket);
	User *user = it->second;

	if (it == this->_users.end())
		return ;
	this->_users.erase(it);

	//채널 삭제 추가 필요
	delete user;

	// close(clientSocket);
	cout << "client disconnected : " << clientSocket << endl;
}

void Server::handleEvent(struct kevent &event)
{
	// 이벤트 유형 구분
	if (event.flags & EV_ERROR) // 에러 이벤트 체크
	{
		if (event.ident == (const uintptr_t)this->_serverSocket)
		{
			cerr << "server socket event error" << endl;
			exit(1);
		}
		else
		{
			cerr << "client socket event error" << endl;
			disconnetClient(event.ident);
		}
	}
	else if (event.filter == EVFILT_READ) {
		// server 소켓이면 연결 요청 수락, client 소켓이면 메시지 수신
		if (event.ident == (const uintptr_t)this->_serverSocket)
			acceptConnection();
		else
			recvMessage(event.ident);
	} else if (event.filter == EVFILT_WRITE) {
		sendMessage(event.ident, "test");
	}
}

void Server::recvMessage(int clientSocket)
{
	char buf[MAX_MESSAGE_SIZE + 1];
	map<int, User *>::iterator it = _users.find(clientSocket);
	User *user = it->second;
	int recvSize;

	if (it == this->_users.end())
		return ;

	// data의 길이가 0이면 연결이 끊긴 것으로 판단, user 삭제
	if ((recvSize = recv(clientSocket, buf, MAX_MESSAGE_SIZE, 0)) <= 0) {
		disconnetClient(clientSocket);
	} else {
		// 메시지 버퍼에 저장
		buf[recvSize] = '\0';
		user->appendMessage(buf);
		handleMessage(user);
	}
}

void Server::sendMessage(int clientSocket, string message)
{
	map<int, User *>::iterator it = this->_users.find(clientSocket);
	User *user = it->second;
	int sendSize;

	if (it == this->_users.end())
		return ;
	if (user->getMessageBuffer().empty())
		return ;

	sendSize = send(clientSocket, message.c_str(), message.length(), 0);
	if (sendSize == -1)
	{
		cerr << "send() error" << endl;
		disconnetClient(clientSocket);
	}
	else
	{
		user->setMessageBuffer(user->getMessageBuffer().substr(sendSize));
	}
}

void Server::handleMessage(User *user)
{
	// clrf가 나올 때까지 메시지를 처리
	while (true) {
		size_t crlfPos = user->getMessageBuffer().find("\r\n", 0);
		
		if (crlfPos == string::npos) {
			break;
		}
		
		if (crlfPos == 0) {
			user->setMessageBuffer(user->getMessageBuffer().substr(1));
			continue;
		}

		Message message(user->getMessageBuffer().substr(0, crlfPos));
		user->clearMessageBuffer();
		// 명령어 처리
		this->_command->handleCommand(message, user);
	}
}
