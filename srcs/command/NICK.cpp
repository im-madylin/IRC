#include "Command.hpp"

//알파벳, 숫자, 밑줄, 대시만 사용 가능
//첫글자는 알파벳이나 밑줄로 시작
bool Command::validNick(string nickname) {
	for (string::iterator it = nickname.begin(); it != nickname.end(); it++) {
		if (!(isalnum(*it) || *it == '_' || *it == '-'))
			return false; 
	}
	if (isdigit(nickname[0]) || nickname[0] == '-')
		return false;
	return true;
}

bool Command::duplicateNick(string nickname) {
	//user 전체 받아와서 중복 검사
	map<int, User *> users = this->_server->getUsers();
	
	for (map<int, User *>::iterator it = users.begin(); it != users.end(); it++) 	{
		User *tmp = it->second;
		if (tmp->getNickname() == nickname)
			return false;
	}
	return true;

}

void Command::NICK(Message &message, User *user) {
	string serverPrefix = this->_server->getServerPrefix();

	//닉네임이 입력되지 않은 경우
	if (message.getParamsSize() < 1) {	
		return user->appendMessage(generateReply(serverPrefix, ERR_NONICKNAMEGIVEN(user->getNickname())));
	}

	string nickname = message.getParams()[0];

	//닉네임이 9글자를 넘어가는 경우, 닉네임이 올바르지 않은 경우
	if (nickname.size() > NICKNAME_MAX_SIZE || validNick(nickname) == false || nickname.size() == 0)
		return user->appendMessage(generateReply(serverPrefix, ERR_ERRONEUSNICKNAME(nickname)));
		
	//이미 사용중인 닉네임인 경우
	else if (duplicateNick(nickname) == false)
		return user->appendMessage(generateReply(serverPrefix, ERR_NICKNAMEINUSE(nickname)));
	
	//정상적인 경우
	else {
		string preNickname = user->getNickname();
		user->setNickname(nickname);
		string msg = ":" + preNickname + "!" + nickname + serverPrefix + " NICK " + nickname + "\r\n";
		
		user->appendMessage(msg);
	}
}
