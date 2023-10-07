#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"

void Command::PART(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();

	if (message.getParamsSize() < 1)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "PART")));
	
	vector<string> channels = split(message.getParams()[0], ",");
	for (vector<string>::iterator it = channels.begin(); it != channels.end(); it++) {
		Channel *channel = this->_server->findChannel(*it);
		
		if (channel == NULL) {
			user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(user->getNickname(), *it)));
			continue ;
		if (!channel->isExistUser(user->getFd())) {
			user->appendMessage(generateReply(serverPrefix, ERR_NOTONCHANNEL(user->getNickname(), *it)));
			continue ;
		}

		this->broadcast(channel, generateReply(userPrefix, " PART " + *it));
		channel->deleteUser(user->getFd());
		user->leaveChannel(*it);
	}
}
