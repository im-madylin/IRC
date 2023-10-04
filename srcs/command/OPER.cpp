#include "Command.hpp"
#include "../Message.hpp"
#include "../user/User.hpp"
#include "../server/Server.hpp"
#include "NumericReplies.hpp"
#include "../channel/Channel.hpp"

void Command::OPER(Message &message, User *user)
{
    string serverPrefix = this->_server->getServerPrefix();

    //ERR_NEEDMOREPARAMS
    if (message.getParamsSize() < 2)
        return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_NEEDMOREPARAMS(user->getNickname(), "OPER")));
    
    string param_user = message.getParams()[0];
    string param_password = message.getParams()[1];

    //ERR_PASSWDMISMATCH
    if (param_password != this->_server->getPassword())
        return sendToClient(user->getFd(), generateReply(serverPrefix, ERR_PASSWDMISMATCH(user->getNickname())));
    
    //TODO: ERR_NOOOPERHOST


    //운영자 권한 설정
    //TODO: 현재는 User *user를 operator로 등록함, param으로 들어오는 user를 등록해야할지, User *user를 등록해야할지는 논의 필요
    
    map<string, Channel *> channels = this->_server->getChannels();

    std::map<std::string, Channel*>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++) {
        Channel* channel = it->second;

        User *tmp = channel->findUser(user->getFd());
        if (tmp != NULL && tmp->getNickname() == user->getNickname())
        {
            channel->addOper(user->getFd());
            break ;
        }
    }

    //TODO: mode o 처리 

    return sendToClient(user->getFd(), generateReply(serverPrefix, RPL_YOUREOPER(user->getNickname())));
}


//ERR_NEEDMOREPARAMS(461)
//RPL_YOUREOPER(381) 운영자 권한을 얻었을때 해당 클라이언트에게 보내짐
//ERR_NOOPERHOST(491) 호스트의 운영자 권한 부여가 서버 설정에 의해 제한되어있는 경우
//ERR_PASSWDMISMATCH(464)