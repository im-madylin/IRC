#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

void Command::NICK(Message &message, User *user)
{
	cout << "NICK: " << message.getParams()[0] << " " << user->getMessageBuffer() << endl;
	user->setNickname(message.getParams()[0]);
}
