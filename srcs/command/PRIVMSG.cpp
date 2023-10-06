#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

void Command::PRIVMSG(Message &message, User *user)
{
	string	serverPrefix = this->_server->getServerPrefix();
	string	userPrefix = user->getUserPrefix();

	if (message.getParamsSize() == 0)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NORECIPIENT(user->getNickname(), "PRIVMSG")));
	if (message.getParamsSize() == 1 || message.getParams()[1].length() == 0)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOTEXTTOSEND(user->getNickname())));
	vector<string>	recipients = split(message.getParams()[0], ",");
	for(vector<string>::const_iterator it = recipients.begin(); it != recipients.end(); it++) {
		if ((*it)[0] == '#') {
			Channel *channel = _server->findChannel(*it);
			if (channel == NULL) {
				sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHNICK(user->getNickname(), *it)));
				continue;
			}
			broadcast(user->getFd(), channel, ":" + userPrefix + " PRIVMSG " + channel->getChannelName() + " :" + message.getParams()[1] + "\r\n");
		}
		else {
			User *recipient = _server->findUser(*it);
			if (recipient == NULL)
				sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHNICK(user->getNickname(), *it)));
			else
				sendToClient(recipient->getFd(),":" + serverPrefix + " PRIVMSG " + *it + " :" + message.getParams()[1]);
		}
	}
}