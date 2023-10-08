#include "Command.hpp"

void Command::PING(Message &message, User *user)
{
	(void) message;
	string serverPrefix = this->_server->getServerPrefix();
	string serverName = this->_server->getServerName();
	string msg = "PING " + serverName + "\r\n";

	sendToClient(user->getFd(), msg);
}


//ERR_NOORIGIN (409)
//ERR_NOSUCHSERVER(402)