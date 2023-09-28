#include "Server.hpp"
#include "../user/User.hpp"
#include "../command/Command.hpp"

Server::Server(string port, string password) 
: _port(atoi(port.c_str())), _password(password)
{
	_serverName = SERVER_NAME;
	_command = new Command(this);
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

		int eventCount = kevent(this->_kq, NULL, 0, this->_events, KQUEUE_SIZE, &timeout);
		if (eventCount == -1)
		{
			cerr << "kevent() error" << endl;
			exit(1);
		}
		for (int i = 0; i < eventCount; i++)
			handleEvent(_events[i]);
	}
}

void Server::initServer()
{
	struct sockaddr_in serverAddr;

	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
	{
		cerr << "socket() error" << endl;
		exit(1);
	}

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	
	int optval = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
	{
		cerr << "setsockopt failed" << endl;
		exit(1);
	}
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
	if (::bind(_serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		cerr << "bind() error" << endl;
		exit(1);
	}

	if (listen(_serverSocket, serverAddr.sin_port) == -1)
	{
		cerr << "listen() error" << endl;
		exit(1);
	}
}

void Server::initKqueue()
{
	if ((_kq = kqueue()) == -1)
	{
		cerr << "kqueue() error" << endl;
		exit(1);
	}
	EV_SET(&this->_change, _serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(this->_kq, &this->_change, 1, NULL, 0, NULL) == -1)
	{
		cerr << "kevent() error" << endl;
		exit(1);
	}
}

void Server::acceptConnection()
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	User *user;

	bzero(&clientAddr, sizeof(clientAddr));
	int clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
	if (clientSocket == -1)
	{
		cerr << "accept() error" << endl;
		exit(1);
	}
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	EV_SET(&this->_change, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(this->_kq, &this->_change, 1, NULL, 0, NULL) == -1)
	{
		cerr << "kevent() error" << endl;
		exit(1);
	}
	cout << "new connection from " << clientSocket << endl;

	user = new User(clientSocket, inet_ntoa(clientAddr.sin_addr));
	_users.insert(make_pair(clientSocket, user));
}

void Server::handleEvent(struct kevent &event)
{
	if (event.filter == EVFILT_READ) {
		if (event.ident == (const uintptr_t)_serverSocket)
			acceptConnection();
		else
			recvMessage(event.ident);
	} else if (event.filter == EVFILT_WRITE) {
		sendMessage(event.ident, "test");
	}
}

void Server::recvMessage(int clientSocket)
{
	char buf[MAX_MESSAGE_SIZE];
	map<int, User *>::iterator it = _users.find(clientSocket);
	User *user = it->second;
	int recvSize;

	if ((recvSize = recv(clientSocket, buf, MAX_MESSAGE_SIZE, 0)) <= 0) {
		_users.erase(it);
		delete user;
	} else {
		buf[recvSize] = '\0';
		user->appendMessage(buf);
		handleMessage(user);
	}
}

void Server::sendMessage(int clientSocket, string message)
{
	if (send(clientSocket, message.c_str(), message.length(), 0) == -1)
		cerr << "send() error" << endl;
}

void Server::handleMessage(User *user)
{
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
		this->_command->handleCommand(message, user);
	}
}
