#include "Command.hpp"

void Command::TOPIC(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();
	
	if (message.getParamsSize() < 1)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "TOPIC")));

	string channelName = message.getParams()[0];
	string topic = message.getParams()[1];

	Channel *channel = this->_server->findChannel(channelName);
	if (channel == NULL)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));

	if (!channel->isExistUser(user->getFd()))
		return user->appendMessage(generateReply(serverPrefix, ERR_NOTONCHANNEL(clientName, channelName)));
	
	if (message.getParamsSize() == 1) {
		if (channel->getTopic().length() == 0)
			return user->appendMessage(generateReply(serverPrefix, RPL_NOTOPIC(clientName, channelName)));
		return user->appendMessage(generateReply(serverPrefix, RPL_TOPIC(clientName, *channel)));
	}

	if (channel->hasMode(CHANNEL_MODE_T) && channel->isOperator(user) == false)
		return user->appendMessage(generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	channel->setTopic(topic);

	string broadcastMessage = " TOPIC " + channelName + " :" + topic;
	this->broadcast(channel, generateReply(userPrefix, broadcastMessage));
}
