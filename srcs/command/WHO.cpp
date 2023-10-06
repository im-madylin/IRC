# include "Command.hpp"
# include "../Message.hpp"
# include "../user/User.hpp"
# include "../server/Server.hpp"

void	Command::WHO(Message &message, User *user)
{
	string	serverPrefix = _server->getServerPrefix();

	if (message.getParamsSize() == 0 || message.getParams()[0] == "0" || message.getParams()[0] == "*") {
		map<int, User *> users = _server->getUsers();
		map<string, Channel *> channels = _server->getChannels();
		for(map<int, User *>::iterator it = users.begin(); it != users.end(); it++) {
			// User *targetUser = it->second;
			//visible 리스트 -> getMode() != "l"
			//rpl whoreply
		}
		return	sendToClient(user->getFd(), generateReply(serverPrefix, RPL_ENDOFWHO(user->getNickname(), "*")));
	}

	bool	operatorListFlag = false;
	if (message.getParamsSize() == 2 && message.getParams()[1] == "o")
		operatorListFlag = true;
	string name = message.getParams()[0];
	Channel *targetChannel = _server->findChannel(name);
	if (targetChannel){
		map<int, User *> users = targetChannel->getUsers();
		for(map<int, User *>::iterator it = users.begin(); it != users.end(); it++) {
			if (operatorListFlag && !targetChannel->isOperator(it->first))
				continue;
			sendToClient(user->getFd(), generateReply(serverPrefix, RPL_WHOREPLY(user->getNickname(), targetChannel->getChannelName(), serverPrefix, *it->second)));
		}
	}
	else {
		map<int, User *> users = _server->getUsers();
		for(map<int, User *>::iterator it = users.begin(); it != users.end(); it++) {
			User *targetUser = it->second;
			if (targetUser->getNickname() == name || targetUser->getUsername() == name || targetUser->getRealName() == name) {
				string channelname = it->second->getJoinedChanels().begin()->first;
				sendToClient(user->getFd(), generateReply(serverPrefix, RPL_WHOREPLY(user->getNickname(), channelname, serverPrefix, *targetUser)));
				break;
			}
		}
	}
	sendToClient(user->getFd(), generateReply(serverPrefix, RPL_ENDOFWHO(user->getNickname(), name)));
}