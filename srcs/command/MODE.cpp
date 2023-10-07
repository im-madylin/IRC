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
	if (message.getParamsSize() < 1)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "MODE")));
	Channel *channel = this->_server->findChannel(channelName);
	if (!user->isInChannel(channelName))
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	if (!channel)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	// TODO: 반환값 수정 필요
	if (message.getParamsSize() == 1) 
		return user->appendMessage(generateReply(serverPrefix, RPL_CHANNELMODEIS(clientName, channelName, channel->getModeString())));
	if (!channel->isOperator(user))
		return user->appendMessage(generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	bool isPlus = true;
	size_t paramIndex = 2;
	string applyMode = "";
	string middle = "";
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
			if (message.getParamsSize() < paramIndex + 1) {
				user->appendMessage(generateReply(serverPrefix, ERR_INVALIDMODEPARAM(clientName, channelName, mode[i], "*", "You must specify a parameter for the key mode. Syntax: <key>")));
				continue ;
			}
			if (isPlus) 
				addKeyMode(channel, message.getParams()[paramIndex]);
			else {
				if (!deleteKeyMode(channel, message.getParams()[paramIndex])) {
					user->appendMessage(generateReply(serverPrefix, ERR_KEYSET(clientName, channelName)));
					continue ;
				}
			}
			middle += message.getParams()[paramIndex] + " ";
			paramIndex++;
		}
		else if (mode[i] == 'o') {
			if (message.getParamsSize() < paramIndex + 1) {
				user->appendMessage(generateReply(serverPrefix, ERR_INVALIDMODEPARAM(clientName, channelName, mode[i], "*", "You must specify a parameter for the op mode. Syntax: <nick>")));
				continue ;
			}
			if (isPlus) {
				// TODO: 존재하지 않는 유저인 경우 처리
				addOperatorMode(channel, message.getParams()[paramIndex]);
			}
			else {
				// TODO: 존재하지 않는 유저인 경우 처리, op가 아닌 경우 무시
				deleteOperatorMode(channel, message.getParams()[paramIndex]);
			} 
			middle += message.getParams()[paramIndex] + " ";
			paramIndex++;
		}
		else if (mode[i] == 'l') {
			if (isPlus) {
				if (message.getParamsSize() < paramIndex + 1) {
					user->appendMessage(generateReply(serverPrefix, ERR_INVALIDMODEPARAM(clientName, channelName, mode[i], "*", "You must specify a parameter for the limit mode. Syntax: <limit>")));
					continue ;
				}
				addLimitMode(channel, atoi(message.getParams()[paramIndex].c_str()));
				middle += message.getParams()[paramIndex] + " ";
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
		else if (mode[i] == 's') {
			if (isPlus)
				channel->addMode(CHANNEL_MODE_S);
			else
				channel->deleteMode(CHANNEL_MODE_S);
		}
		else if (mode[i] == 'n') {
			if (isPlus)
				channel->addMode(CHANNEL_MODE_N);
			else
				channel->deleteMode(CHANNEL_MODE_N);
		}
		else {
			user->appendMessage(generateReply(serverPrefix, ERR_UNKNOWNMODE(clientName, mode[i], channelName)));
			continue ;
		}
		applyMode += mode[i];
	}
	if (containsOnlyPlusMinus(applyMode))
		return ;
	// TODO: 수정 필요
	string trailing = "";

	broadcast(channel, generateReply(userPrefix, "MODE " + channelName + " " +  + " :" + applyMode));
}

void Command::addKeyMode(Channel *channel, string key)
{
	channel->addMode(CHANNEL_MODE_K);
	channel->setKey(key);
}

bool Command::deleteKeyMode(Channel *channel, string key)
{
	channel->deleteMode(CHANNEL_MODE_K);
	if (channel->getKey() != key)
		return false;
	channel->setKey("");
	return true;
}

void Command::addOperatorMode(Channel *channel, string targetName)
{
	User *target = channel->findUser(targetName);
	//TODO: 아예 무시되도록 처리
	if (target == NULL || channel->isOperator(target))
		return ;
	channel->addMode(CHANNEL_MODE_O);
	channel->addOper(target->getFd());
}

void Command::deleteOperatorMode(Channel *channel, string targetName)
{
	User *target = channel->findUser(targetName);
	//TODO: 아예 무시되도록 처리
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

bool Command::containsOnlyPlusMinus(string mode)
{
	for (size_t i = 0; i < mode.size(); i++)
		if (mode[i] != '+' && mode[i] != '-')
			return false;
	return true;
}
