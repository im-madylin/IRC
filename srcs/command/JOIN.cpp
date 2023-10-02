#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"
#include "NumericReplies.hpp"

void Command::JOIN(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	// ERR_NEEDMOREPARAMS
	if (message.getParamsSize() < 2)
		return user->setMessageBuffer(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN")));

	string delimeter = ",";
	vector<string> channels = split(message.getParams()[0], delimeter);
	
	vector<string> keys;
	if (message.getParamsSize() > 2)
		keys = split(message.getParams()[1], delimeter);
	while (keys.size() < channels.size())
		keys.push_back("");
	
	for (vector<string> ::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)[0] != '#') {
			// MEMO: 필요한지..?
			user->setMessageBuffer(generateReply(serverPrefix, ERR_NOSUCHCHANNEL(user->getNickname(), *it)));
			continue ;
		}
		Channel *channel = this->_server->findChannel(*it);
		if (channel == NULL) {
			// ERR_TOOMANYCHANNELS
			if (this->_server->getChannels().size() >= MAX_CHANNEL_SIZE)
				return user->setMessageBuffer(generateReply(serverPrefix, ERR_TOOMANYCHANNELS(user->getNickname(), *it)));
			channel = new Channel(*it);
			this->_server->addChannel(channel);
			channel->addOper(user->getFd());
		}
		// 이미 있는 유저는 무시
		if (!channel->isExistUser(user->getFd())) {
			if (channel->isFull()) {
				user->setMessageBuffer(generateReply(serverPrefix, ERR_CHANNELISFULL(user->getNickname(), *it)));
				continue ;
			}
			// TODO: 수정 필요 invite only 
			if (channel->getMode() == "i") {
				user->setMessageBuffer(generateReply(serverPrefix, ERR_INVITEONLYCHAN(user->getNickname(), *it)));
				continue ;
			}

			// TODO: ERR_BANNEDFROMCHAN

			// TODO: ERR_BADCHANNELKEY

			channel->addUser(user->getFd(), user);
			user->joinChannel(channel);

			// RPL_TOPIC
			if (channel->getTopic().length() > 0)
				user->setMessageBuffer(generateReply(serverPrefix, RPL_TOPIC(*it, *channel)));
			// RPL_NAMREPLY
			user->setMessageBuffer(generateReply(serverPrefix, RPL_NAMREPLY(user->getNickname(), *channel)));
		}
	}

	// TODO: ERR_NOSUCHCHANNEL
}
