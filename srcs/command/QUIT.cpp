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
        string str = "";
        for (size_t i = 0; i < message.getParamsSize(); i++)
        {
            if (i != 0)
                str += " ";
            str += message.getParams()[i];
        }
        user->appendMessage(msg + str + "\r\n");
    }

    this->_server->disconnetClient(user->getFd());    
}