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
}

Command::~Command()
{
}

void Command::handleCommand(Message &message, User *user)
{
	string command = message.getCommand();
	if (_commands.find(command) != _commands.end()) {
		(this->*_commands[command])(message, user);
	}
}
