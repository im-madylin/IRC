#include "Command.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"
#include "../Message.hpp"

Command::Command(Server *server) : _server(server)
{
	_commands["PASS"] = &Command::PASS;
	_commands["PING"] = &Command::PING;
	_commands["NICK"] = &Command::NICK;
	_commands["USER"] = &Command::USER;
	_commands["JOIN"] = &Command::JOIN;
	_commands["PART"] = &Command::PART;
	_commands["TOPIC"] = &Command::TOPIC;
	_commands["PRIVMSG"] = &Command::PRIVMSG;
	_commands["NOTICE"] = &Command::NOTICE;
	_commands["KICK"] = &Command::KICK;
	_commands["INVITE"] = &Command::INVITE;
	_commands["OPER"] = &Command::OPER;
	_commands["QUIT"] = &Command::QUIT;
}

Command::~Command()
{
}

void Command::handleCommand(Message &message, User *user)
{
	string command = message.getCommand();

	if (user->getAuth() == true || command == "PASS")
	{
		cout << "here!" << endl;
		if (_commands.find(command) != _commands.end()) {
			(this->*_commands[command])(message, user);
		}
	}
	else
	{
		cout << "You must get Auth" << endl;
	}
}

// 임시로 만든 함수
void Command::sendToClient(int fd, string message)
{
	send(fd, message.c_str(), message.length(), 0);
}

void Command::broadcast(int ignoreFd, Channel *channel, string message)
{
	map<int, User *> user = channel->getUsers();
	for(map<int, User *>::iterator it = user.begin(); it != user.end(); it++) {
		if (it->first == ignoreFd)
			continue;
		sendToClient(it->first, message);
	}
}

void Command::broadcast(Channel *channel, string message)
{
	map<int, User *> user = channel->getUsers();
	for(map<int, User *>::iterator it = user.begin(); it != user.end(); it++) {
		sendToClient(it->first, message);
	}
}
