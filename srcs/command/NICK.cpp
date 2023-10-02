#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"

//알파벳, 숫자, 밑줄, 대시만 사용 가능
//첫글자는 알파벳이나 밑줄로 시작
bool Command::validNick(string nickname) 
{
	for (string::iterator it = nickname.begin(); it != nickname.end(); ++it)
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
	// map<int, User *> users = *_server->getUsers();
	(void)nickname;
	return true;

}

void Command::NICK(Message &message, User *user)
{
	cout << "NICK: " << message.getParams()[0] << " " << user->getMessageBuffer() << endl;

	string nickname = message.getParams()[0];

	//닉네임이 입력되지 않은 경우
	if (nickname.size() == 0)
		cout << "NICK: ERR_NONICKNAMEGIVEN(431)" << endl;
	//닉네임이 9글자를 넘어가는 경우, 닉네임이 올바르지 않은 경우
	else if (nickname.size() > NICKNAME_MAX_SIZE || validNick(nickname) == false)
		cout << "NICK: ERR_ERRONEUSNICKNAME(432)" << endl;
	//이미 사용중인 닉네임인 경우
	else if (duplicateNick(nickname) == false)
		cout << "NICK: ERR_NICKNAMEINUSE(433)" << endl;
	//정상적인 경우
	else
	{
		user->setNickname(nickname);
		cout << "You’re now known as " << nickname << endl;
	}
}

//닉네임이 입력되지 않은 경우, ERR_NONICKNAMEGIVEN(431)
//닉네임이 9글자를 넘어가는 경우 ERR_ERRONEUSNICKNAME(432)
//사용중인 닉네임인 경우 ERR_NICKNAMEINUSE(433)
//닉네임이 충돌했을 경우 ERR_COLLISION(436)