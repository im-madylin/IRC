#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

// channel mode
// i(invite only), t(topic), k(key, channel password), o(operator), l(user limit) 구현
void Command::MODE(Message &message, User *user)
{
	string	serverPrefix = this->_server->getServerPrefix();
	string	userPrefix = user->getUserPrefix();
	string	clientName = user->getNickname();
	string	channelName = message.getParams()[0];
	string	mode = message.getParams()[1];
	if (message.getParamsSize() < 2)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "MODE")));
	
	Channel *channel = this->_server->findChannel(channelName);
	if (!user->isInChannel(channelName))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	if (!channel->isOperator(user))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	//ERR_KEYSET

	// :*.freenode.net 472 eunbi x :is not a recognised channel mode.
	// ERR_UNKNOWNMODE
	bool isPlus = true;
	size_t paramIndex = 2;
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '+')
			isPlus = true;
		else if (mode[i] == '-')
			isPlus = false;
		else if (mode[i] == 'i') {
			if (isPlus)
				channel->addMode(CHANNEL_MODE_I);
			else
				channel->deleteMode(CHANNEL_MODE_I);
		}
		else if (mode[i] == 't') {
			if (isPlus)
				channel->addMode(CHANNEL_MODE_T);
			else
				channel->deleteMode(CHANNEL_MODE_T);
		}	
		else if (mode[i] == 'k') {
			if (message.getParamsSize() < paramIndex + 1)
					return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_INVALIDMODEPARAM(clientName, channelName, mode[i], "*", "You must specify a parameter for the key mode. Syntax: <key>")));
			if (isPlus) 
				addKeyMode(channel, message.getParams()[paramIndex]);
			else
				deleteKeyMode(channel, message.getParams()[paramIndex]);
			paramIndex++;
		}
		else if (mode[i] == 'o') {
			if (message.getParamsSize() < paramIndex + 1)
					return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_INVALIDMODEPARAM(clientName, channelName, mode[i], "*", "You must specify a parameter for the op mode. Syntax: <nick>")));
			if (isPlus)
				addOperatorMode(channel, message.getParams()[paramIndex]);
			else 
				deleteOperatorMode(channel, message.getParams()[paramIndex]);
			paramIndex++;
		}
		else if (mode[i] == 'l') {
			if (isPlus) {
				if (message.getParamsSize() < paramIndex + 1)
					return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_INVALIDMODEPARAM(clientName, channelName, mode[i], "*", "You must specify a parameter for the limit mode. Syntax: <limit>")));
				addLimitMode(channel, atoi(message.getParams()[paramIndex].c_str()));
				paramIndex++;
			}
			else
				deleteLimitMode(channel);
		}
		else if (mode[i] == 'b') {
			if (isPlus)
				channel->addMode(CHANNEL_MODE_B);
			else
				channel->deleteMode(CHANNEL_MODE_B);
		}
		else
			return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_UNKNOWNMODE(clientName, mode[i], channelName)));
	}
	broadcast(channel, generateReply(userPrefix, "MODE " + channelName + " :" + mode));
}

void Command::addKeyMode(Channel *channel, string key)
{
	channel->addMode(CHANNEL_MODE_K);
	channel->setKey(key);
}

void Command::deleteKeyMode(Channel *channel, string key)
{
	channel->deleteMode(CHANNEL_MODE_K);
	if (channel->getKey() == key)
	channel->setKey(NULL);
}

void Command::addOperatorMode(Channel *channel, string targetName)
{
	User *target = channel->findUser(targetName);
	if (target == NULL || channel->isOperator(target))
		return ;
	channel->addMode(CHANNEL_MODE_O);
	channel->addOper(target->getFd());
}

void Command::deleteOperatorMode(Channel *channel, string targetName)
{
	User *target = channel->findUser(targetName);
	if (target == NULL || channel->isOperator(target))
		return ;
	channel->deleteMode(CHANNEL_MODE_O);
	channel->deleteOper(target->getFd());
}

void Command::addLimitMode(Channel *channel, int limit)
{
	channel->addMode(CHANNEL_MODE_L);
	channel->setLimit(limit);
}

void Command::deleteLimitMode(Channel *channel)
{
	channel->deleteMode(CHANNEL_MODE_L);
	channel->setLimit(0);
}
