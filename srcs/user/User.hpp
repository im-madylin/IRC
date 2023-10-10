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
		string	_username;
		string	_realName;
		string 	_messageBuffer;
		string 	_commandBuffer;
		bool	_isRegistered;
		map<string, Channel *> _joinedChannels;
		bool	_auth;

	public:
		User(int fd, string host);
		~User();

		string	getMessageBuffer() const;
		string 	getCommandBuffer() const;
		int		getFd() const;
		string	getHost() const;
		string	getNickname() const;
		string	getUsername() const;
		string	getRealName() const;
		string	getUserPrefix() const;
		bool	getIsRegistered() const;
		map<string, Channel *>	getJoinedChannels() const;
		bool	getAuth() const;
		
		void 	setMessageBuffer(string messageBuffer);
		void 	setCommandBuffer(string commandBuffer);
		void	setFd(int fd);
		void	setHost(string host);
		void	setNickname(string nick);
		void	setUsername(string username);
		void	setRealName(string realName);
		void	setRegistered();
		void	setAuth();

		void	clearMessageBuffer();
		void	clearCommandBuffer();
		void	appendMessage(const string &message);
		void	appendCommand(const string &command);
		void	joinChannel(Channel *channel);
		void	leaveChannel(string channelName);
		bool	isInChannel(string channelName);
};

#endif
