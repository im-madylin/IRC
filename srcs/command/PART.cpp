#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"

void Command::PART(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();
	if (message.getParamsSize() < 1)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "PART")));
	
	vector<string> channels = split(message.getParams()[0], ",");
	for (vector<string>::iterator it = channels.begin(); it != channels.end(); it++) {
		Channel *channel = this->_server->findChannel(*it);
		
		if (channel == NULL) {
			sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOSUCHCHANNEL(user->getNickname(), *it)));
			continue ;
		}
		
		if (!channel->isExistUser(user->getFd())) {
			sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NOTONCHANNEL(user->getNickname(), *it)));
			continue ;
		}
		
		channel->deleteUser(user->getFd());
		user->leaveChannel(*it);
	}
}

// TODO: 같은 채널에 있는 유저들에게 broadcast
