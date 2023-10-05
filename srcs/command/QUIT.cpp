#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"
#include "NumericReplies.hpp"

void Command::QUIT(Message &message, User *user)
{
    string serverPrefix = this->_server->getServerPrefix();

    string msg = ":" + serverPrefix + " 001 " + user->getNickname() + " :QUIT GoodBye!" + "\r\n";
	
	
    if (message.getParamsSize() < 1)
        sendToClient(user->getFd(), msg + "GoodBye");
    else
    {
        //TODO: message의 params이 들어오는대로 msg를 출력해야하나? ex)QUIT go to lunch 인 경우 msg를 QUIT: go to lunch 로..?

        //일단 Param이 1개로 가정
        sendToClient(user->getFd(), msg + message.getParams()[0]);
    }

    //TODO: msg 보낸 후, 클라이언트와 접속 끊기
    //MEMO: 따로 처리안했는데, /QUIT 명령어를 하면 약 3~5초 뒤에 limechat에서 disconnect 처리됨
    cout << "disconnect client" << endl;
}