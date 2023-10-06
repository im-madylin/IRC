#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"
#include "NumericReplies.hpp"

//알파벳, 숫자, 밑줄, 대시만 사용 가능
//첫글자는 알파벳이나 밑줄로 시작
bool Command::validNick(string nickname) 
{
	for (string::iterator it = nickname.begin(); it != nickname.end(); it++)
	{
        if (!(isalnum(*it) || *it == '_' || *it == '-'))
			return false; 
    }
	if (isdigit(nickname[0]) || nickname[0] == '-')
		return false;
	return true;
}

bool Command::duplicateNick(string nickname)
{
	//user 전체 받아와서 중복 검사
	map<int, User *> users = _server->getUsers();
	
	for (map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		User *tmp = it->second;
		if (tmp->getNickname() == nickname)
			return false;
	}
	return true;

}


void Command::NICK(Message &message, User *user)
{
	string serverPrefix = this->_server->getServerPrefix();

	cout << "size: " << message.getParamsSize() << endl;
	//닉네임이 입력되지 않은 경우
	if (message.getParamsSize() < 1)
	{	
		return user->appendMessage(generateReply(serverPrefix, ERR_NONICKNAMEGIVEN(user->getNickname())));
	}	// return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NONICKNAMEGIVEN(nickname)));


	string nickname = message.getParams()[0];

	//닉네임이 9글자를 넘어가는 경우, 닉네임이 올바르지 않은 경우
	if (nickname.size() > NICKNAME_MAX_SIZE || validNick(nickname) == false)
		return user->appendMessage(generateReply(serverPrefix, ERR_ERRONEUSNICKNAME(nickname)));
		// return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_ERRONEUSNICKNAME(nickname)));
	// TODO: 연속으로 같은 닉네임(hi)을 입력했을 경우, 첫번째는 오류와 함께 You're now known as hi 라고 출력
	// 두번째는 오류과 함께 You're now known as hi_ 라고 출력 (언더바가 생겨난 모습)
	//이미 사용중인 닉네임인 경우
	else if (duplicateNick(nickname) == false)
		return user->appendMessage(generateReply(serverPrefix, ERR_NICKNAMEINUSE(nickname)));
		// return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NICKNAMEINUSE(nickname)));
	//정상적인 경우
	else
	{
		user->setNickname(nickname);
		string result = "You’re now known as " + user->getNickname() + "\r\n";
		// TODO: 결과 msg가 뜨지 않음
		// sendToClient(user->getFd(), result);
		user->appendMessage(result);
	}
}

//닉네임이 입력되지 않은 경우, ERR_NONICKNAMEGIVEN(431)
//닉네임이 9글자를 넘어가는 경우 ERR_ERRONEUSNICKNAME(432)
//사용중인 닉네임인 경우 ERR_NICKNAMEINUSE(433)
//닉네임이 충돌했을 경우 ERR_COLLISION(436) 
