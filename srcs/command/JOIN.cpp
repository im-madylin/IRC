#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"
#include "NumericReplies.hpp"

void Command::JOIN(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 1)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN")));

	string delimeter = ",";
	vector<string> channels = split(message.getParams()[0], delimeter);
	
	vector<string> keys;
	if (message.getParamsSize() >= 2)
		keys = split(message.getParams()[1], delimeter);
	while (keys.size() < channels.size())
		keys.push_back("");
	
	for (vector<string> ::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = this->_server->findChannel(*it);
		if (channel == NULL) {
			if ((*it)[0] != '#') {
				sendToClient(user->getFd(), generateReply(serverPrefix, ERR_BADCHANMASK(user->getNickname(), *it)));
				continue ;
			}
			// ERR_TOOMANYCHANNELS
			if (this->_server->getChannels().size() >= MAX_CHANNEL_SIZE)
				return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_TOOMANYCHANNELS(user->getNickname(), *it)));
			channel = new Channel(*it);
			this->_server->addChannel(channel);
			channel->addOper(user->getFd());
			user->joinChannel(channel);

			this->broadcast(channel, generateReply(userPrefix, " JOIN :" + *it));
			// RPL_NAMREPLY
			sendToClient(user->getFd(), generateReply(serverPrefix, RPL_NAMREPLY(user->getNickname(), *channel)));
			continue ;
		}
		// 이미 있는 유저는 무시
		if (channel->isExistUser(user->getFd()))
			continue ;
		if (channel->isFull()) {
			sendToClient(user->getFd(), generateReply(serverPrefix, ERR_CHANNELISFULL(user->getNickname(), *it)));
			continue ;
		}
		// TODO: 수정 필요 invite only 
		if (channel->getMode() == "i") {
			if (!channel->isInvited(user->getFd())) {
				sendToClient(user->getFd(), generateReply(serverPrefix, ERR_INVITEONLYCHAN(user->getNickname(), *it)));
				continue ;
			}
		}

		// TODO: 수정 필요
		if (channel->getMode() == "b") {
			if (channel->isInBanList(user->getFd())) {
				sendToClient(user->getFd(), generateReply(serverPrefix, ERR_BANNEDFROMCHAN(user->getNickname(), *it)));
				continue ;
			}
		}

		// TODO: 수정 필요
		if (channel->getMode() == "k") {
			if (channel->getKey() != keys[it - channels.begin()]) {
				sendToClient(user->getFd(), generateReply(serverPrefix, ERR_BADCHANNELKEY(user->getNickname(), *it)));
				continue ;
			}
		}

		channel->addUser(user->getFd(), user);
		user->joinChannel(channel);

		this->broadcast(channel, generateReply(userPrefix, " JOIN :" + *it));
		// RPL_TOPIC
		if (channel->getTopic().length() > 0)
			sendToClient(user->getFd(), generateReply(serverPrefix, RPL_TOPIC(*it, *channel)));
		// RPL_NAMREPLY
		sendToClient(user->getFd(), generateReply(serverPrefix, RPL_NAMREPLY(user->getNickname(), *channel)));
	}
}
