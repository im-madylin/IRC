#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>
# include <vector>
# include "../user/User.hpp"

class Server;
class User;

using namespace std;

class Channel {
	private:
		string				_name;
		map<int, User *>	_users;
		set<int> 			_operator;

	public:
		Channel();
		Channel(std::string& name);
		~Channel();

		string			getChannelName();
		vector<string>	getUserList();

		void		addUser(int fd, User *user);
		void		delUser(int fd);
		User*		findUser(int fd) const;
		User*		findUser(string nickname) const;
		bool		isOperator(int fd) const;
		bool		isOperator(User *user) const;
		void		addOper(int fd);
		void		delOper(int fd);
};

#endif
