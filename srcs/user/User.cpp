#include "User.hpp"
#include "../channel/Channel.hpp"

User::User(int fd, string host): _fd(fd), _host(host), _isRegistered(false)
{
}

User::~User()
{
	close(_fd);
}

string User::getMessageBuffer()
{
	return this->_messageBuffer;
}

string User::getCommandBuffer()
{
	return this->_commandBuffer;
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

string User::getUsername()
{
	return this->_username;
}

string User::getRealName()
{
	return this->_realName;
}

string User::getUserPrefix()
{
	return this->_nickname + "!" + this->_realName + "@" + this->_host;
}

bool User::getIsRegistered()
{
	return this->_isRegistered;
}

void User::setMessageBuffer(string messageBuffer)
{
	this->_messageBuffer = messageBuffer;
}

void User::setCommandBuffer(string commandBuffer)
{
	this->_commandBuffer = commandBuffer;
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

void User::setUsername(string username)
{
	this->_username = username;
}

void User::setRealName(string realName)
{
	this->_realName = realName;
}

void User::setRegistered()
{
	this->_isRegistered = true;
}

void User::clearMessageBuffer()
{
	this->_messageBuffer.clear();
}

void User::clearCommandBuffer()
{
	this->_commandBuffer.clear();
}

void User::appendMessage(const string &message)
{
	this->_messageBuffer.append(message);
}

void User::appendCommand(const string &command)
{
	this->_commandBuffer.append(command);
}

void User::joinChannel(Channel *channel)
{ 
	this->_joinedChannels.insert(make_pair(channel->getChannelName(), channel));
}

void User::leaveChannel(string channelName)
{
	this->_joinedChannels.erase(channelName);
}

bool User::isInChannel(string channelName)
{
	if (this->_joinedChannels.find(channelName) != this->_joinedChannels.end())
		return true;
	return false;
}
