#ifndef USER_HPP
# define USER_HPP

# include <string>

using namespace std;

class User {
	private:
		int		_fd;
		string	_host;
		string	_nickname;
		string	_realName;

		string 	_messageBuf;

	public:
		User(int fd, string host);
		~User();

		string	getMessageBuf();
		int		getFd();
		string	getHost();
		string	getNickname();
		string	getRealName();

		void 	setMessageBuf(string messageBuf);
		void	setFd(int fd);
		void	setHost(string host);
		void	setNickname(string nick);
		void	setRealName(string realName);

		void	clearMessageBuf();
		void	appendMessage(const string &message);
};

#endif
