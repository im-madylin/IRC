# include "Command.hpp"

void	Command::WHO(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();

	if (message.getParamsSize() == 0 || message.getParams()[0].length() == 0)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "WHO")));

	bool operatorListFlag = false;
	if (message.getParamsSize() == 2 && message.getParams()[1] == "o")
		operatorListFlag = true;
	string name = message.getParams()[0];
	Channel *targetChannel = this->_server->findChannel(name);
	if (targetChannel) {
		map<int, User *> users = targetChannel->getUsers();
		for(map<int, User *>::iterator it = users.begin(); it != users.end(); it++) {
			if (operatorListFlag && !targetChannel->isOperator(it->first))
				continue;
			user->appendMessage(generateReply(serverPrefix, RPL_WHOREPLY(user->getNickname(), targetChannel->getChannelName(), serverPrefix, *it->second)));
		}
	}
	else {
		map<int, User *> users = this->_server->getUsers();
		for(map<int, User *>::iterator it = users.begin(); it != users.end(); it++) {
			User *targetUser = it->second;
			if (targetUser->getNickname() == name || targetUser->getUsername() == name || targetUser->getRealName() == name) {
				string channelname = it->second->getJoinedChannels().begin()->first;
				user->appendMessage(generateReply(serverPrefix, RPL_WHOREPLY(user->getNickname(), channelname, serverPrefix, *targetUser)));
				break;
			}
		}
	}
	user->appendMessage(generateReply(serverPrefix, RPL_ENDOFWHO(user->getNickname(), name)));
}
