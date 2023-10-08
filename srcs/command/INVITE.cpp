#include "Command.hpp"

void Command::INVITE(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();
	
	if (message.getParamsSize() < 2)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "INVITE")));
	string targetName = message.getParams()[0];
	string channelName = message.getParams()[1];
	
	Channel *channel = this->_server->findChannel(channelName);
	if (channel == NULL)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	
	User *target = this->_server->findUser(targetName);
	if (target == NULL)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHNICK(clientName, channelName)));

	if (channel->isExistUser(target->getFd()))
		return user->appendMessage(generateReply(serverPrefix, ERR_USERONCHANNEL(clientName, targetName, channelName)));
	
	// invite-only 채녈인 경우는 operator만 가능
	if (channel->hasMode(CHANNEL_MODE_I) && channel->isOperator(user) == false)
		return user->appendMessage(generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, targetName)));
	
	channel->addInvite(target->getFd());

	user->appendMessage(generateReply(serverPrefix, RPL_INVITING(clientName, targetName, channelName)));
	target->appendMessage(generateReply(userPrefix, " INVITE " + targetName + " :" + channelName));
}
