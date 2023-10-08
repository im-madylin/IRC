#include "Command.hpp"

void Command::QUIT(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();
	string msg = ":" + user->getNickname() + "!" + user->getNickname() + serverPrefix + " QUIT ";
		
	if (message.getParamsSize() < 1)
		user->appendMessage(msg + "GoodBye" + "\r\n");
	else {
		string str = "";
		for (size_t i = 0; i < message.getParamsSize(); i++) {
			if (i != 0)
				str += " ";
			str += message.getParams()[i];
		}
		user->appendMessage(msg + str + "\r\n");
	}

	this->_server->disconnetClient(user->getFd());
}