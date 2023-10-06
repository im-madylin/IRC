#include "Command.hpp"
#include "../server/Server.hpp"
#include "../channel/Channel.hpp"
#include "../Message.hpp"
#include "NumericReplies.hpp"

void Command::QUIT(Message &message, User *user)
{
    string serverPrefix = this->_server->getServerPrefix();

    string msg = ":" + serverPrefix + " 001 " + user->getNickname() + " :QUIT ";
	
    if (message.getParamsSize() <= 1 && message.getParams()[0] == "")
        sendToClient(user->getFd(), msg + "GoodBye " + "\r\n");
    else
    {
        //TODO: message의 params이 들어오는대로 msg를 출력해야하나? ex)QUIT go to lunch 인 경우 msg를 QUIT: go to lunch 로..?

        //일단 Param이 1개로 가정
        sendToClient(user->getFd(), msg + message.getParams()[0] + "\r\n");
    }

    this->_server->disconnetClient(user->getFd());    
}