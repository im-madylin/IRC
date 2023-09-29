#include "User.hpp"

User::User(int fd, string host): _fd(fd), _host(host)
{
	this->_fd = fd;
}

User::~User()
{
}

string User::getMessageBuffer()
{
	return this->_messageBuffer;
}

int User::getFd()
{
	return this->_fd;
}

string User::getHost()
{
	return this->_host;
}

string User::getNickname()
{
	return this->_nickname;
}

string User::getRealName()
{
	return this->_realName;
}

void User::setMessageBuffer(string messageBuffer)
{
	this->_messageBuffer = messageBuffer;
}

void User::setFd(int fd)
{
	this->_fd = fd;
}

void User::setHost(string host)
{
	this->_host = host;
}

void User::setNickname(string nick)
{
	this->_nickname = nick;
}

void User::setRealName(string realName)
{
	this->_realName = realName;
}

void User::clearMessageBuffer()
{
	this->_messageBuffer.clear();
}

void User::appendMessage(const string &message)
{
	_messageBuffer.append(message);
}