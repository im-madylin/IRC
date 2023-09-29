#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# include <map>

class Server;
class User;
class Message;

using namespace std;

class Command {
	private:
		typedef void(Command::*_pfunc)(Message &message, User *user);

		Server		*_server;
		map<string, _pfunc>	_commands;

		void	PASS(Message &message, User *user);
		void	PING(Message &message, User *user);
		void	NICK(Message &message, User *user);
		void	USER(Message &message, User *user);
	public:
		Command(Server *server);
		~Command();
		void 	handleCommand(Message &message, User *user);
};

#endif