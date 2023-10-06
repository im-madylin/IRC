#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

// <nickname>!<username>@<host>
void Command::USER(Message &message, User *user)
{
	string	serverPrefix = this->_server->getServerPrefix();
	if (message.getParams().size() < 4)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
		// return user->appendCommand(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
	string username = message.getParams()[0];
	string realname = message.getParams()[3];
	
	if (user->getIsRegistered())
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_ALREADYREGISTERED(user->getNickname())));
		// return user->appendCommand(generateReply(serverPrefix, ERR_ALREADYREGISTERED(user->getNickname())));
	if (message.getParams().size() < 4)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
		// return user->appendCommand(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
	
	user->setUsername(username);
	user->setRealName(realname);
	user->setRegistered();
	string welcome = ":" + serverPrefix + " 001 " + username + " :Welcome to the Internet Relay Network ~" + user->getNickname() + "!" + username + "@" + user->getHost() + "\r\n";
	// user->appendCommand(welcome);
	sendToClient(user->getFd(), welcome);
	cout << "USER: " << username << " " << user->getHost() << " " << realname << endl;
}
