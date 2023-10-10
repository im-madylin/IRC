#include "Server.hpp"
#include "../user/User.hpp"
#include "../command/Command.hpp"
#include "../channel/Channel.hpp"

/* ---------------------------------- PRIVATE ---------------------------------- */

Server::Server(int port, string password) : _port(port), _password(password) {
	this->_serverName = SERVER_NAME;
	this->_command = new Command(this);
	initServer();
}

Server::~Server() {
	deleteAllUser();
	deleteAllChannel();
	delete this->_command;
	close(this->_serverSocket);
}

void Server::initServer() {
	struct sockaddr_in serverAddr;

	// server 소켓 생성 AF_INET: IPv4, SOCK_STREAM: TCP
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocket == -1) {
		errorExit("socket failed");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(this->_port); // 호스트 바이트 순서로 표현된 숫자를 네트워크 바이트 순서로 변환
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP 주소로부터의 연결을 허용, 네트워크 바이트 순서로 32비트 정수를 변환
	
	// 소켓 옵션 설정
	// SOL_SOCKET: 소켓 옵션 레벨, 일반 소켓 옵션
	// SO_REUSEADDR: 커널이 소켓을 사용하는 중에도 포트를 사용할 수 있게 해줌
	int optval = 1;
	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		errorExit("setsockopt failed");
	}

	// 파일 관련 작업을 수행
	// non-blocking 모드로 변경
	fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);

	// 해당 주소와 server로 들어오는 클라이언트의 연결을 수락할 수 있도록 함
	// 바인딩할 주소 정보
	if (bind(this->_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		errorExit("bind failed");
	}
	
	// 연결 요청 대기열 생성
	if (listen(this->_serverSocket, WAITING_QUEUE_SIZE) == -1) {
		errorExit("listen failed");
	}
}

void Server::initKqueue() {
	// kqueue 생성
	if ((this->_kq = kqueue()) == -1) {
		errorExit("kqueue failed");
	}

	// 이벤트 설정 -> 읽기 이벤트 감지, 이벤트 추가, 이벤트 활성화
	updateKevent(this->_serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

void Server::updateKevent(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata) {
	struct kevent change;

	EV_SET(&change, ident, filter, flags, fflags, data, udata);
	this->_changes.push_back(change);
}

void Server::handleEvent(struct kevent &event) {
	// 이벤트 유형 구분
	if (event.flags & EV_ERROR) {
		// 에러 발생한 소켓이 서버일 때는 서버 닫고 종료, 클라이언트일 때는 소켓 닫기
		if (event.ident == (const uintptr_t)this->_serverSocket) {
			errorExit("server socket");
		} else {
			cerr << "Error: client socket" << endl;
			disconnectClient(event.ident);
		}
	} else if (event.filter == EVFILT_READ) {
		// server 소켓이면 연결 요청 수락, client 소켓이면 메시지 수신
		if (event.ident == (const uintptr_t)this->_serverSocket)
			acceptConnection();
		else
			recvMessage(event.ident);
	} else if (event.filter == EVFILT_WRITE) {
		sendMessage(event.ident);
	}
}

// client 연결 수락 및 소켓 생성
void Server::acceptConnection() {
	// client 소켓 정보 저장할 구조체
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	User *user;

	// 클라이언트 연결 수락 및 소켓 생성
	memset(&clientAddr, 0, sizeof(clientAddr));
	int clientSocket = accept(this->_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
	if (clientSocket == -1) {
		cerr << "Error: accept failed" << endl;
		return ;
	}
	// non-blocking 모드로 변경
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);

	// kqueue에 등록하여 client 소켓을 감시
	updateKevent(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	updateKevent(clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
	cout << "new connection from : " << clientSocket << endl;

	// user 객체 생성 및 서버에 저장
	// inet_ntoa: 네트워크 바이트 순서의 32비트 정수를 IP 주소 문자열로 변환	
	// sin_addr: 32비트 IP 주소
	user = new User(clientSocket, inet_ntoa(clientAddr.sin_addr));
	addUser(user);
}

void Server::deleteAllUser(){
	map<int, User *>::iterator it;

	for (it = this->_users.begin(); it != this->_users.end(); it++) {
		deleteUser(it->first);
	}
}

void Server::deleteAllChannel() {
	map<string, Channel *>::iterator it;

	for (it = this->_channels.begin(); it != this->_channels.end(); it++) {
		deleteChannel(it->first);
	}
}

void Server::recvMessage(int clientSocket) {
	char buf[MAX_MESSAGE_SIZE + 1];
	map<int, User *>::iterator it = this->_users.find(clientSocket);
	User *user = it->second;
	int recvSize;

	if (it == this->_users.end())
		return ;

	if ((recvSize = recv(clientSocket, buf, MAX_MESSAGE_SIZE + 1, 0)) <= 0) {
		cerr << "Error: recv failed" << endl;
		disconnectClient(clientSocket);
	} else {
		buf[recvSize] = '\0';
		user->appendCommand(buf);
		handleCmdMessage(user);
	}
}

void Server::handleCmdMessage(User *user) {
	while (true) {
		size_t crlfPos = findCRLF(user->getCommandBuffer());
		
		if (crlfPos == string::npos) {
			break;
		}
		
		if (crlfPos == 0) {
			user->setCommandBuffer(user->getCommandBuffer().substr(1));
			continue;
		}

		// crlf 위치까지의 문자열을 Message 객체로 생성 (message 안에서 파싱)
		Message message(user->getCommandBuffer().substr(0, crlfPos));
		user->setCommandBuffer(user->getCommandBuffer().substr(crlfPos + 1));
		this->_command->handleCommand(message, user);
	}
}

size_t Server::findCRLF(string message) {
	size_t crPos = message.find("\r");
	size_t lfPos = message.find("\n");

	if (lfPos == string::npos) return crPos;
	if (crPos == string::npos) return lfPos;
	return min(crPos, lfPos);
}

/* ----------------------------------- PUBLIC ---------------------------------- */

int Server::getPort() const {
	return this->_port;
}

string Server::getServerName() const {
	return this->_serverName;
}

string Server::getPassword() const {
	return this->_password;
}

map<int, User *> Server::getUsers() const {
	return this->_users;
}

string Server::getServerPrefix() const {
	return this->_serverName;
}

map<string, Channel *> Server::getChannels() const {
	return this->_channels;
}

void Server::setServerName(string serverName) {
	this->_serverName = serverName;
}

void Server::run() {
	initKqueue();
	while (1) {

		// kqueue에 등록된 이벤트가 발생할 때까지 대기, 이벤트 수만큼 반환
		// changes : 이벤트를 설정하거나 변경하기 위한 kevent 구조체 배열
		// events : 발생한 이벤트를 저장할 kevent 구조체 배열
		int eventCount = kevent(this->_kq, &this->_changes[0], this->_changes.size(), this->_events, KQUEUE_SIZE, NULL);
		if (eventCount == -1) 		{
			deleteAllUser();
			close(this->_serverSocket);
			errorExit("kevent failed");
		}

		// 큐에 담은 이벤트 삭제
		this->_changes.clear();

		// 발생한 이벤트 처리
		for (int i = 0; i < eventCount; i++)
			handleEvent(_events[i]);
	}
}

void Server::sendMessage(int clientSocket) {
	map<int, User *>::iterator it = this->_users.find(clientSocket);
	User *user = it->second;
	int sendSize;

	if (it == this->_users.end())
		return ;
	if (user->getMessageBuffer().empty())
		return ;

	// 소켓으로 메세지 전송
	// fcntl로 소켓을 논블로킹으로 설정했기 때문에 send가 블로킹되지 않음
	sendSize = send(clientSocket, user->getMessageBuffer().c_str(), user->getMessageBuffer().length(), 0);
	if (sendSize == -1) {
		cerr << "Error: send failed" << endl;
		disconnectClient(clientSocket);
	}
	else {
		user->setMessageBuffer(user->getMessageBuffer().substr(sendSize));
	}
}

void Server::disconnectClient(int clientSocket) {
	map<int, User *>::iterator it = this->_users.find(clientSocket);
	User *user = it->second;

	if (it == this->_users.end())
		return ;
	
	map<string, Channel *> _joinedChannels = user->getJoinedChannels();
	for (map<string, Channel *>::iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); it++) {
		it->second->deleteUser(user->getFd());
		user->leaveChannel(it->first);
		if (it->second->getUsers().empty())
			this->deleteChannel(it->first);
	}
	cout << "client disconnected : " << clientSocket << endl;

	delete this->_users[clientSocket];
	this->_users.erase(it);
}

void Server::addChannel(Channel *channel) {
	this->_channels.insert(make_pair(channel->getChannelName(), channel));
}

Channel *Server::findChannel(string channelName) {
	for(map<string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		if (it->second->getChannelName() == channelName)
			return it->second;
	return NULL;
}

void Server::deleteChannel(string channelName) {
	map<string, Channel *>::iterator it = this->_channels.find(channelName);
	if (it == this->_channels.end())
		return ;

	delete it->second;
	this->_channels.erase(channelName);
}

User *Server::findUser(string username) {
	for(map<int, User *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
		if (it->second->getNickname() == username)
			return it->second;
	return NULL;
}

void Server::addUser(User *user) {
	this->_users.insert(make_pair(user->getFd(), user));
}

void Server::deleteUser(int clientFd) {
	map<int, User *>::iterator it = this->_users.find(clientFd);
	if (it == this->_users.end())
		return ;

	delete it->second;
	this->_users.erase(clientFd);
}
