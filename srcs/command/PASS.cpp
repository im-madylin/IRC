#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"
#include "NumericReplies.hpp"

void Command::PASS(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();

	string password = message.getParams()[0];

	//파라미터가 없는 경우
	if (password.size() == 0)
		return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "PASS")));
	
	//TODO: 이미 등록되어 있는 경우
	
	//TODO: 수정 필요
	//server password와 비교
	if (password == this->_server->getPassword())
		user->setAuth();
	else
		cout << "PASS: password error" << endl;
}

//파라미터의 수가 부족할 때 ERR_NEEDMOREPARAMS(461)
//이미 등록되어 있는 상태일 때 ERR_ALREADYREGISTERD(462)