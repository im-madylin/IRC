#include "Command.hpp"

// channel mode
// i(invite only), t(topic), k(key, channel password), o(operator), l(user limit) 구현
void Command::MODE(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();

	if (message.getParamsSize() < 1)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "MODE")));

	string channelName = message.getParams()[0];
	Channel *channel = this->_server->findChannel(channelName);
	if (!user->isInChannel(channelName))
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	if (!channel)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	if (message.getParamsSize() == 1) 
		return user->appendMessage(generateReply(serverPrefix, RPL_CHANNELMODEIS(clientName, channelName, channel->getModeString())));
	if (!channel->isOperator(user))
		return user->appendMessage(generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	string mode = message.getParams()[1];
	bool isPlus = true;
	size_t paramIndex = 2;
	string applyMode = "";
	string middle = " ";
	for (size_t i = 0; i < mode.size(); i++) {
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
				if (!addOperatorMode(channel, user, message.getParams()[paramIndex]))
					continue ;
			}
			else {
				if (!deleteOperatorMode(channel, user, message.getParams()[paramIndex]))
					continue ;
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
	string params = " " + applyMode + middle;
	broadcast(channel, generateReply(userPrefix, MODE_REPLY(channelName, params)));
}

void Command::addKeyMode(Channel *channel, string key) {
	channel->addMode(CHANNEL_MODE_K);
	channel->setKey(key);
}

bool Command::deleteKeyMode(Channel *channel, string key) {
	channel->deleteMode(CHANNEL_MODE_K);
	if (channel->getKey() != key)
		return false;
	channel->setKey("");
	return true;
}

bool Command::addOperatorMode(Channel *channel, User *user, string targetName) {
	if (!this->_server->findUser(targetName)) {
		user->appendMessage(generateReply(this->_server->getServerPrefix(), ERR_NOSUCHNICK(user->getNickname(), targetName)));
		return false;
	}
	User *target = channel->findUser(targetName);
	if (!target || channel->isOperator(target))
		return false;
	channel->addMode(CHANNEL_MODE_O);
	channel->addOper(target->getFd());
	return true;
}

bool Command::deleteOperatorMode(Channel *channel, User *user, string targetName) {
	User *target = channel->findUser(targetName);
	if (!this->_server->findUser(targetName)) {
		user->appendMessage(generateReply(this->_server->getServerPrefix(), ERR_NOSUCHNICK(user->getNickname(), targetName)));
		return false;
	}
	if (!target || !channel->isOperator(target))
		return false;
	channel->deleteMode(CHANNEL_MODE_O);
	channel->deleteOper(target->getFd());
	return true;
}

void Command::addLimitMode(Channel *channel, int limit) {
	channel->addMode(CHANNEL_MODE_L);
	channel->setLimit(limit);
}

void Command::deleteLimitMode(Channel *channel) {
	channel->deleteMode(CHANNEL_MODE_L);
	channel->setLimit(0);
}

bool Command::containsOnlyPlusMinus(string mode) {
	for (size_t i = 0; i < mode.size(); i++)
		if (mode[i] != '+' && mode[i] != '-')
			return false;
	return true;
}
