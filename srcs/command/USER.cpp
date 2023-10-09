#include "Command.hpp"

// <nickname>!<username>@<host>
void Command::USER(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();
	
	if (message.getParams().size() < 4)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
	
	string username = message.getParams()[0];
	string realname = message.getParams()[3];
	if (user->getIsRegistered())
		return user->appendMessage(generateReply(serverPrefix, ERR_ALREADYREGISTERED(user->getNickname())));
	if (message.getParams().size() < 4)
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "USER")));
	
	user->setUsername(username);
	user->setRealName(realname);
	user->setRegistered();
	string welcome = ":" + serverPrefix + " 001 " + username + " :Welcome to the Internet Relay Network ~" + user->getNickname() + "!" + username + "@" + user->getHost() + "\r\n";
	user->appendMessage(welcome);
	welcomMessage(user);
	cout << "USER: " << username << " " << user->getHost() << " " << realname << endl;
}
