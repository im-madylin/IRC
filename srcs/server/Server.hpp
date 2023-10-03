#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/event.h>
# include <sys/time.h>
# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <fcntl.h>

# include "../Message.hpp"

# define SERVER_NAME "irc.easy"
# define MAX_MESSAGE_SIZE 512
# define MAX_CHANNEL_SIZE 10
# define UNDEFINED -1
# define KQUEUE_SIZE 8
# define KQUEUE_TIMEOUT 180

class User;
class Channel;
class Command;

using namespace std;

class Server {
	private:
		int		_kq;
		int		_port; // client 서버 접속시 필요한 Port 번호
		int		_serverSocket;
		string	_password;
		string	_serverName;

		struct kevent _events[KQUEUE_SIZE]; // 이벤트 저장 배열
		map<int, User *> _users;
		map<string, Channel *> _channels;
		struct kevent _change;
		Command*			_command;

		void	initServer();
		void	initKqueue();
		void	acceptConnection();
		void	disconnetClient(int clientFd);
		void	handleEvent(struct kevent &event);
	
		void	recvMessage(int clientSocket);
		size_t	findCRLF(string message);
		void	handleCmdMessage(User *user);

	public:
		Server(string port, string password);
		~Server();

		int		getPort() const;
		string	getServerName() const;
		string	getPassword() const;
		map<int, User *>	getUsers() const;
		string 	getServerPrefix() const;
		map<string, Channel *> getChannels() const;

		void	setServerName(string serverName);

		void	run();
		void	sendMessage(int clientSocket);
		void	addChannel(Channel *channel);
		Channel *findChannel(string channelName); // TODO: 구현 필요
		void	deleteChannel(string channelName);
};

#endif
