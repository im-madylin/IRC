#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"
#include "NumericReplies.hpp"

void Command::TOPIC(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 1)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "TOPIC")));
	
	Channel *channel = this->_server->findChannel(message.getParams()[0]);
	if (channel == NULL)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(user->getNickname(), message.getParams()[0])));

	if (!channel->isExistUser(user->getFd()))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOTONCHANNEL(user->getNickname(), message.getParams()[0])));
	
	// 파라미터로 topic이 들어오지 않은 경우
	if (message.getParamsSize() == 1) {
		if (channel->getTopic() == "")
			return sendToClient(user->getFd(), generateReply(serverPrefix, RPL_NOTOPIC(user->getNickname(), message.getParams()[0])));
		return sendToClient(user->getFd(), generateReply(serverPrefix, RPL_TOPIC(user->getNickname(), *channel)));
	}

	if (!channel->isOperator(user))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(user->getNickname(), message.getParams()[0])));
	
	string topic = message.getParams()[1];
	channel->setTopic(topic);
	sendToClient(user->getFd(), generateReply(serverPrefix, RPL_TOPIC(user->getNickname(), *channel)));
	// broadcast
}
