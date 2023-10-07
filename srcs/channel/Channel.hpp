#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>
# include <vector>
# include "../user/User.hpp"
# include "../bot/Bot.hpp"

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
	CHANNEL_MODE_B,
	CHANNEL_MODE_S,
	CHANNEL_MODE_N,
};

class Channel {
	private:
		string				_name;
		map<int, User *>	_users;
		set<int> 			_operator;
		string				_topic;
		set<ChannelMode>	_modes;
		set<int>			_banList;
		set<int>			_inviteList;
		string				_key;
		size_t				_limit;
		Bot					_bot;

	public:
		Channel();
		Channel(std::string& name);
		~Channel();

		string			getChannelName();
		map<int, User *>getUsers();
		vector<string>	getUserList();
		string			getTopic();
		set<ChannelMode> getModes();
		set<int>		getBanList();
		set<int>		getInviteList();
		string			getKey();
		string			getModeString();
		size_t			getLimit();

		void			setTopic(string topic);
		void			setKey(string key);
		void			setLimit(int limit);

		void		addUser(int fd, User *user);
		void		deleteUser(int fd);
		User*		findUser(int fd) const;
		User*		findUser(string nickname) const;
		bool		isOperator(int fd) const;
		bool		isOperator(User *user) const;
		void		addOper(int fd);
		void		deleteOper(int fd);
		void		addBan(int fd);
		void		deleteBan(int fd);
		void		addInvite(int fd);
		void		deleteInvite(int fd);
		bool		isExistUser(int fd) const;
		bool		isFull() const;
		bool		hasMode(ChannelMode mode) const;
		void		addMode(ChannelMode mode);
		void		deleteMode(ChannelMode mode);
		bool		isInvited(int fd) const;
		bool		isInBanList(int fd) const;
		string		executeBot(string date);
};

#endif
