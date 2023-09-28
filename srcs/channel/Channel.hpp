#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>

class Server;
class User;

using namespace std;

class Channel {
	private:
		// Server	*_server;
		// User	*_operator;
		// string	_name;

		// set<User *>	_users;

	public:
		Channel();
		~Channel();
};

#endif
