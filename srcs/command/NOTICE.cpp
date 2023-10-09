#include "Command.hpp"

void Command::NOTICE(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();

	if (message.getParamsSize() == 0)
		return user->appendMessage(generateReply(serverPrefix, ERR_NORECIPIENT(user->getNickname(), "NOTICE")));
	if (message.getParamsSize() == 1 || message.getParams()[1].length() == 0)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOTEXTTOSEND(user->getNickname())));

	vector<string> recipients = split(message.getParams()[0], ",");
	for(vector<string>::const_iterator it = recipients.begin(); it != recipients.end(); it++) {
		if ((*it)[0] == '#') {
			Channel *channel = this->_server->findChannel(*it);
			if (channel == NULL)
				continue;
			broadcast(user->getFd(), channel, ":" + userPrefix + " NOTICE " + channel->getChannelName() + " :" + message.getParams()[1] + "\r\n");
		}
		else {
			User *recipient = this->_server->findUser(*it);
			if (recipient == NULL)
				continue;
			else
				recipient->appendMessage(":" + serverPrefix + " NOTICE " + *it + " :" + message.getParams()[1] + "\r\n");
		}
	}
}