#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <unistd.h>
# include <map>

using namespace std;

class Channel;

class User {
	private:
		int		_fd;
		string	_host;
		string	_nickname;
		string	_realName;
		string 	_messageBuffer;
		string 	_commandBuffer;

		map<string, Channel *> _joinedChannels;

	public:
		User(int fd, string host);
		~User();

		string	getMessageBuffer();
		string 	getCommandBuffer();
		int		getFd();
		string	getHost();
		string	getNickname();
		string	getRealName();
		string	getUserPrefix();

		void 	setMessageBuffer(string messageBuffer);
		void 	setCommandBuffer(string commandBuffer);
		void	setFd(int fd);
		void	setHost(string host);
		void	setNickname(string nick);
		void	setRealName(string realName);

		void	clearMessageBuffer();
		void	clearCommandBuffer();
		void	appendMessage(const string &message);
		void	appendCommand(const string &command);
		void	joinChannel(Channel *channel);
};

#endif
