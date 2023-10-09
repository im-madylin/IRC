#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <sys/types.h>
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
# define KQUEUE_SIZE 10
# define WAITING_QUEUE_SIZE 10

class User;
class Channel;
class Command;

using namespace std;

class Server {
	private:
		int						_kq;
		int						_port;
		int						_serverSocket;
		string					_password;
		string					_serverName;

		struct kevent			_events[KQUEUE_SIZE]; // 이벤트 저장 배열
		map<int, User *>		_users;
		map<string, Channel *>	_channels;
		vector<struct kevent>	_changes;
		Command*				_command;

		void					initServer();
		void					initKqueue();
		void					updateKevent(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
		void					handleEvent(struct kevent &event);
		void					acceptConnection();
		void					deleteAllUser();
		void					deleteAllChannel();
		void					recvMessage(int clientSocket);
		void					sendMessage(int clientSocket);
		void					handleCmdMessage(User *user);
		size_t					findCRLF(string message);

	public:
		Server(int port, string password);
		~Server();

		int						getPort() const;
		string					getServerName() const;
		string					getPassword() const;
		map<int, User *>		getUsers() const;
		string					getServerPrefix() const;
		map<string, Channel *>	getChannels() const;

		void					setServerName(string serverName);

		void					run();
		void					disconnectClient(int clientFd);
		void					addChannel(Channel *channel);
		Channel					*findChannel(string channelName);
		void					deleteChannel(string channelName);
		User					*findUser(string username);
		void					addUser(User *user);
		void					deleteUser(int clientFd);
};

#endif
