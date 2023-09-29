#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"

void Command::PASS(Message &message, User *user)
{
	cout << "PASS: " << message.getParams()[0] << " " << user->getMessageBuffer() << endl;
}
