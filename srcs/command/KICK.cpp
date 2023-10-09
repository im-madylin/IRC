#include "Command.hpp"

void Command::KICK(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();
	
	if (message.getParamsSize() < 2)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "KICK")));
	string channelName = message.getParams()[0];
	string targetName = message.getParams()[1];
	
	Channel *channel = this->_server->findChannel(channelName);
	if (!channel)
		return user->appendMessage(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));

	User *target = channel->findUser(targetName);
	if (!target)
		return user->appendMessage(generateReply(serverPrefix, ERR_USERNOTINCHANNEL(clientName, targetName, channelName)));
	
	if (!channel->findUser(clientName))
		return user->appendMessage(generateReply(serverPrefix, ERR_NOTONCHANNEL(clientName, channelName)));

	if (!channel->isOperator(user))
		return user->appendMessage(generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	string broadcastMessage = "KICK " + channelName + " " + targetName + " :";
	this->broadcast(channel, generateReply(userPrefix, broadcastMessage));

	channel->deleteUser(target->getFd());
	target->leaveChannel(channelName);
	if (channel->isInvited(target->getFd()))
		channel->deleteInvite(target->getFd());
	if (channel->getUsers().empty())
		this->_server->deleteChannel(channel->getChannelName());
}
