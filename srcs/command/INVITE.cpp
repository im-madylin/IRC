#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"

void Command::INVITE(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();
	string targetName = message.getParams()[0];
	string channelName = message.getParams()[1];
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 2)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "INVITE")));
	
	Channel *channel = this->_server->findChannel(channelName);
	if (channel == NULL)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(clientName, channelName)));
	
	User *target = this->_server->findUser(targetName);
	if (target == NULL)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHNICK(clientName, channelName)));

	if (channel->isExistUser(target->getFd()))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_USERONCHANNEL(clientName, targetName, channelName)));
	
	// invite-only 채녈인 경우는 operator만 가능
	if (channel->hasMode(CHANNEL_MODE_I) && channel->isOperator(user) == false)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(clientName, targetName)));
	
	channel->addInvite(target->getFd());

	// MEMO: 나머지 멤버한테도 알려야 하나?
	sendToClient(user->getFd(), generateReply(serverPrefix, RPL_INVITING(clientName, targetName, channelName)));
	sendToClient(target->getFd(), generateReply(userPrefix, " INVITE " + targetName + " :" + channelName));
}
