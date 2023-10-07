#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"
#include "NumericReplies.hpp"

void Command::JOIN(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	string userPrefix = user->getUserPrefix();
	string clientName = user->getNickname();

	if (message.getParamsSize() < 1)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(clientName, "JOIN")));

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
			if (this->_server->getChannels().size() >= MAX_CHANNEL_SIZE) {
				user->appendMessage(generateReply(serverPrefix, ERR_TOOMANYCHANNELS(clientName, *it)));
				continue;
			}

			channel = new Channel(*it);
			this->_server->addChannel(channel);
			channel->addOper(user->getFd());
			channel->addUser(user->getFd(), user);
			user->joinChannel(channel);

			this->broadcast(channel, generateReply(userPrefix, " JOIN :" + *it));

			user->appendMessage(generateReply(serverPrefix, RPL_NAMREPLY(clientName, *channel)));
			user->appendMessage(generateReply(serverPrefix, RPL_ENDOFNAMES(clientName, *it)));
		} else {
			if (channel->isExistUser(user->getFd()))
				continue ;
			
			if (channel->hasMode(CHANNEL_MODE_L)) {
				if (channel->isFull() && channel->isInvited(user->getFd()) == false) {
					user->appendMessage(generateReply(serverPrefix, ERR_CHANNELISFULL(clientName, *it)));
					continue ;
				}
			}

			if (channel->hasMode(CHANNEL_MODE_K)) {
				if (channel->getKey() != keys[it - channels.begin()]) {
					user->appendMessage(generateReply(serverPrefix, ERR_BADCHANNELKEY(clientName, *it)));
					continue ;
				}
			}
			
			if (channel->hasMode(CHANNEL_MODE_I)) {
				if (!channel->isInvited(user->getFd())) {
					user->appendMessage(generateReply(serverPrefix, ERR_INVITEONLYCHAN(clientName, *it)));
					continue ;
				}
			}

			if (channel->hasMode(CHANNEL_MODE_B)) {
				if (channel->isInBanList(user->getFd())) {
					user->appendMessage(generateReply(serverPrefix, ERR_BANNEDFROMCHAN(clientName, *it)));
					continue ;
				}
			}

			channel->addUser(user->getFd(), user);
			user->joinChannel(channel);

			this->broadcast(channel, generateReply(userPrefix, " JOIN :" + *it));

			if (channel->getTopic().length() > 0)
				user->appendMessage(generateReply(serverPrefix, RPL_TOPIC(*it, *channel)));

			user->appendMessage(generateReply(serverPrefix, RPL_NAMREPLY(clientName, *channel)));
			user->appendMessage(generateReply(serverPrefix, RPL_ENDOFNAMES(clientName, *it)));
		}
	}	
}
