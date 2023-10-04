#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"
#include "NumericReplies.hpp"

void Command::OPER(Message &message, User *user)
{
    string serverPrefix = this->_server->getServerPrefix();

    //ERR_NEEDMOREPARAMS
    if (message.getParamsSize() < 2)
        return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "OPER")));
    
    string user = message.getParams()[0];
    string password = message.getParams()[1];

    //ERR_PASSWDMISMATCH
    if (password != this->_server->getPassword())
        return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_PASSWDMISMATCH(user->getNickname())));
    
    //TODO: ERR_NOOOPERHOST


    //TODO: 운영자 권한 설정 및 MODE o 처리하기
    
    return sendToClient(user->getFd(), generateReply(serverPrefix, RPL_YOUREOPER(user->getNickname())));
}


//ERR_NEEDMOREPARAMS(461)
//RPL_YOUREOPER(381) 운영자 권한을 얻었을때 해당 클라이언트에게 보내짐
//ERR_NOOPERHOST(491) 호스트의 운영자 권한 부여가 서버 설정에 의해 제한되어있는 경우
//ERR_PASSWDMISMATCH(464)