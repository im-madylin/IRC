#include "Command.hpp"

void Command::PASS(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();

	//파라미터가 없는 경우
	if (message.getParamsSize() < 1 || message.getParams()[0] == "")
		return user->appendMessage(generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "PASS")));
	
	if (user->getAuth() == true)
		return user->appendMessage(generateReply(serverPrefix, ERR_ALREADYREGISTERED(user->getNickname())));

	//server password와 비교
	string password = message.getParams()[0];

	if (password == this->_server->getPassword())
		user->setAuth();
	else
		cout << "PASS: password error" << endl;
}
