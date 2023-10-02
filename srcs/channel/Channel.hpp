#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>
# include <vector>
# include "../user/User.hpp"

# define MAX_CHANNEL_USER 10

class Server;
class User;

using namespace std;

class Channel {
	private:
		string				_name;
		map<int, User *>	_users;
		set<int> 			_operator;
		string				_topic;
		string				_mode;

	public:
		Channel();
		Channel(std::string& name);
		~Channel();

		string			getChannelName();
		vector<string>	getUserList();
		string			getTopic();
		string			getMode();

		void			setTopic(string topic);
		void			setMode(string mode);

		void		addUser(int fd, User *user);
		void		deleteUser(int fd);
		User*		findUser(int fd) const;
		User*		findUser(string nickname) const;
		bool		isOperator(int fd) const;
		bool		isOperator(User *user) const;
		void		addOper(int fd);
		void		deleteOper(int fd);
		bool		isExistUser(int fd) const;
		bool		isFull() const;
};

#endif
