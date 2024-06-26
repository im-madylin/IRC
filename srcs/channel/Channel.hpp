#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>
# include <vector>

# include "../user/User.hpp"
# include "../bot/Bot.hpp"
# include "../Utils.hpp"

# define MAX_CHANNEL_USER 10

class Server;
class User;

using namespace std;

enum ChannelMode {
	CHANNEL_MODE_I,
	CHANNEL_MODE_T,
	CHANNEL_MODE_K,
	CHANNEL_MODE_O,
	CHANNEL_MODE_L,
	CHANNEL_MODE_S, // Secret Channel Mode
	CHANNEL_MODE_N, // No External Messages Mode
};

class Channel {
	private:
		string				_name;
		map<int, User *>	_users;
		set<int> 			_operator;
		string				_topic;
		set<ChannelMode>	_modes;
		set<int>			_inviteList;
		string				_key;
		size_t				_limit;
		Bot					_bot;

	public:
		Channel();
		Channel(std::string& name);
		~Channel();

		string				getChannelName() const;
		map<int, User *>	getUsers() const;
		vector<string>		getUserList();
		string				getTopic() const;
		set<ChannelMode>	getModes() const;
		set<int>			getInviteList() const;
		string				getKey() const;
		string				getModeString();
		size_t				getLimit() const;

		void				setTopic(string topic);
		void				setKey(string key);
		void				setLimit(int limit);

		void				addUser(int fd, User *user);
		void				deleteUser(int fd);
		User*				findUser(int fd) const;
		User*				findUser(string nickname) const;
		bool				isOperator(int fd) const;
		bool				isOperator(User *user) const;
		void				addOper(int fd);
		void				deleteOper(int fd);
		bool				isExistUser(int fd) const;
		bool				isFull() const;
		bool				hasMode(ChannelMode mode) const;
		void				addMode(ChannelMode mode);
		void				deleteMode(ChannelMode mode);
		bool				isInvited(int fd) const;
		void				addInvite(int fd);
		void				deleteInvite(int fd);
		string				executeBot(string date);
};

#endif
