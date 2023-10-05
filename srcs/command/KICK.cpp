#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"

// :irc.local 401 eunbi go :No such nick
// :eunbi!root@127.0.0.1 KICK #go test :
// :irc.local 441 peach test #go :They are not on that channel
// :irc.local 442 test #go :You're not on that channel!
void Command::KICK(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();
	string channelName = message.getParams()[0];
	string targetName = message.getParams()[1];
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 2)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "KICK")));
	
	Channel *channel = this->_server->findChannel(channelName);
	if (!channel)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));

	User *target = channel->findUser(targetName);
	if (!target)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_USERNOTINCHANNEL(clientName, targetName, channelName)));
	
	if (!channel->findUser(clientName))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOTONCHANNEL(clientName, channelName)));

	if (!channel->isOperator(user))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, channelName)));
	
	channel->deleteUser(target->getFd());
	target->leaveChannel(channelName);
	
	string broadcastMessage = " KICK " + channelName + " " + targetName + " :";
	this->broadcast(channel, generateReply(userPrefix, broadcastMessage));
}
