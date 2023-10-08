#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <map>

# include "../Utils.hpp"
# include "../server/Server.hpp"
# include "../channel/Channel.hpp"
# include "../user/User.hpp"
# include "../Message.hpp"
# include "NumericReplies.hpp"

# define NICKNAME_MAX_SIZE 9
# define CHANNEL_MODE_I_STR "i"
# define CHANNEL_MODE_T_STR "t"
# define CHANNEL_MODE_K_STR "k"
# define CHANNEL_MODE_O_STR "o"
# define CHANNEL_MODE_L_STR "l"
# define CHANNEL_MODE_B_STR "b"
# define CHANNEL_MODE_S_STR "s"
# define CHANNEL_MODE_N_STR "n"

class Server;
class User;
class Message;

using namespace std;

class Command {
	private:
		typedef void(Command::*_pfunc)(Message &message, User *user);

		Server				*_server;
		map<string, _pfunc>	_commands;

		void				PASS(Message &message, User *user);
		void				PING(Message &message, User *user);
		void				NICK(Message &message, User *user);
		void				USER(Message &message, User *user);
		void				JOIN(Message &message, User *user);
		void				TOPIC(Message &message, User *user);
		void				PART(Message &message, User *user);
		void				PRIVMSG(Message &message, User *user);
		void				NOTICE(Message &message, User *user);
		void				KICK(Message &message, User *user);
		void				INVITE(Message &message, User *user);
		void				QUIT(Message &message, User *user);
		void				WHO(Message &message, User *user);
		void				MODE(Message &message, User *user);

		void				welcomMessage(User *user);

		// MODE.cpp
		void				addKeyMode(Channel *channel, string key);
		void				addLimitMode(Channel *channel, int limit);
		bool				addOperatorMode(Channel *channel, User *user, string nickname);

		bool				deleteKeyMode(Channel *channel, string key);
		void				deleteLimitMode(Channel *channel);
		bool				deleteOperatorMode(Channel *channel, User *user, string nickname);

		bool				containsOnlyPlusMinus(string mode);
	
	public:
		Command(Server *server);
		~Command();
		void 				handleCommand(Message &message, User *user);
		void				sendToClient(int fd, string message);
		void				broadcast(int ignoreFd, Channel *channel, string message);
		void				broadcast(Channel *channel, string message);
	
		//NICK.cpp
		bool				validNick(string nickname);
		bool				duplicateNick(string nickname);
};

#endif
