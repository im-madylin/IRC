#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

void Command::PING(Message &message, User *user)
{
	(void) message;
	string serverName = this->_server->getServerName();
	string msg = serverName + "PONG" + serverName + ":" + serverName + "\r\n";
	cout << msg << endl;
	this->_server->sendMessage(user->getFd(), msg);
}
