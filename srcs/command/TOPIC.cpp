#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"

void Command::TOPIC(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();
	string channelName = message.getParams()[0];
	string topic = message.getParams()[1];
	string broadcastMessage = " TOPIC " + channelName + " :" + topic;
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 1)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "TOPIC")));
	
	Channel *channel = this->_server->findChannel(channelName);
	if (channel == NULL)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));

	if (!channel->isExistUser(user->getFd()))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOTONCHANNEL(clientName, channelName)));
	
	// 파라미터로 topic이 들어오지 않은 경우
	if (message.getParamsSize() == 1) {
		if (channel->getTopic() == "")
			return sendToClient(user->getFd(), generateReply(serverPrefix, RPL_NOTOPIC(clientName, channelName)));
		return sendToClient(user->getFd(), generateReply(serverPrefix, RPL_TOPIC(clientName, *channel)));
	}

	if (!channel->isOperator(user))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	channel->setTopic(topic);
	this->broadcast(channel, generateReply(userPrefix, broadcastMessage));
}
