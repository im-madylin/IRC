#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

void Command::USER(Message &message, User *user)
{
	string username = message.getParams()[0];
	string hostname = message.getParams()[1];
	string servername = message.getParams()[2];
	string realname = message.getParams()[3];
	user->setNickname(username);
	user->setHost(hostname);
	this->_server->setServerName(servername);
	user->setRealName(realname);
	string welcome = ":irc.easy 001 " + username + " :Welcome to the Internet Relay Network " + username + "!" + hostname + "@" + servername + "\r\n";
	sendToClient(user->getFd(), welcome);
	cout << "USER: " << username << " " << hostname << " " << servername << " " << realname << endl;
}
