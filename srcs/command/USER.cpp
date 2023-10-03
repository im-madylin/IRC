#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

void Command::USER(Message &message, User *user)
{
	string	serverPrefix = this->_server->getServerPrefix();
	if (message.getParams().size() < 4)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
	
	string username = message.getParams()[0];
	string hostname = message.getParams()[1];
	string servername = message.getParams()[2];
	string realname = message.getParams()[3];
	
	if (user->getIsRegistered())
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_ALREADYREGISTERED(user->getNickname())));
	if (message.getParams().size() < 4)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
	
	user->setNickname(username);
	user->setHost(hostname);
	user->setRealName(realname);
	string welcome = ":" + serverPrefix + " 001 " + username + " :Welcome to the Internet Relay Network ~" + username + "!" + hostname + "@" + servername + "\r\n";
	sendToClient(user->getFd(), welcome);
	cout << "USER: " << username << " " << hostname << " " << servername << " " << realname << endl;
}