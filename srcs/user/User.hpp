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

		string 	_messageBuffer;

	public:
		User(int fd, string host);
		~User();

		string	getMessageBuffer();
		int		getFd();
		string	getHost();
		string	getNickname();
		string	getRealName();

		void 	setMessageBuffer(string messageBuffer);
		void	setFd(int fd);
		void	setHost(string host);
		void	setNickname(string nick);
		void	setRealName(string realName);

		void	clearMessageBuffer();
		void	appendMessage(const string &message);
};

#endif
