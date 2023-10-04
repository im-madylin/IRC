#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"

void Command::INVITE(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 2)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "INVITE")));
	
	Channel *channel = this->_server->findChannel(message.getParams()[1]);
	if (channel == NULL)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(user->getNickname(), message.getParams()[0])));
	
	User *target = this->_server->findUser(message.getParams()[0]);
	if (target == NULL)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHNICK(user->getNickname(), message.getParams()[1])));

	if (channel->isExistUser(user->getFd()))
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_USERONCHANNEL(user->getNickname(), message.getParams()[0], message.getParams()[1])));
	
	// invite-only 채녈인 경우는 operator만 가능
	if (channel->getMode() == "i" && channel->isOperator(user) == false)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANOPRIVSNEEDED(user->getNickname(), message.getParams()[0])));
	
	channel->addInvite(target->getFd());
	// broadcast

	sendToClient(user->getFd(), generateReply(serverPrefix, RPL_INVITING(user->getNickname(), target->getNickname(), channel->getChannelName())));
}
