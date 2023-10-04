#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string& name) : _name(name) {
}

Channel::~Channel()
{
}

string  		Channel::getChannelName() {
    return _name;
}

map<int, User *>Channel::getUsers()
{
	return _users;
}

vector<string>  Channel::getUserList() {
    vector<string>  list;

    for(map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++) {
        string  name;
        if (isOperator(it->first))
            name += '@';
        name += it->second->getNickname(); 
        list.push_back(name);
    }
    return list;
}

string Channel::getTopic()
{
	return this->_topic;
}

string Channel::getMode()
{
	return this->_mode;
}

set<int> Channel::getBanList()
{
	return this->_banList;
}

set<int> Channel::getInviteList()
{
	return this->_inviteList;
}

void Channel::setTopic(string topic)
{
	this->_topic = topic;
}

string Channel::getKey()
{
	return this->_key;
}

void Channel::setMode(string mode)
{
	this->_mode = mode;
}

void        	Channel::addUser(int fd, User *user) {
    if (_users.empty())
        _operator.insert(fd);
    _users.insert(make_pair(fd, user));
}

void			Channel::deleteUser(int fd) {
	map<int, User *>::iterator it;

	if (isOperator(fd))
		deleteOper(fd);
	
	it = _users.find(fd);
	if (it == _users.end())
		return ;
	_users.erase(fd);
}

User*			Channel::findUser(int fd) const {
	map<int, User *>::const_iterator it;

	it = _users.find(fd);
	if (it == _users.end())
		return NULL;
	return it->second;
}

User*			Channel::findUser(string nickname) const {
	map<int, User *>::const_iterator it;

	for(it = _users.begin(); it != _users.end(); it++)
		if (!it->second->getNickname().compare(nickname))
			return it->second;
	return it->second;
}

void Channel::setKey(string key)
{
	this->_key = key;
}

bool			Channel::isOperator(int fd) const {
	if (_operator.find(fd) != _operator.end())
		return true;
	return false;
}

bool			Channel::isOperator(User *user) const {
	for(set<int>::iterator it = _operator.begin(); it != _operator.end(); it++)
		if (*it == user->getFd())
			return true;
	return false;
}

void			Channel::addOper(int fd) {
	_operator.insert(fd);
}

void			Channel::deleteOper(int fd) {
	_operator.erase(fd);
}

bool		Channel::isExistUser(int fd) const {
	if (_users.find(fd) == _users.end())
		return false;
	return true;
}

bool Channel::isFull() const
{
	if (_users.size() >= MAX_CHANNEL_USER)
		return true;
	return false;
}

bool Channel::isInvited(int fd) const
{
	if (_inviteList.find(fd) != _inviteList.end())
		return true;
	return false;
}

bool Channel::isInBanList(int fd) const
{
	if (_banList.find(fd) != _banList.end())
		return true;
	return false;
}

void Channel::addBan(int fd)
{
	this->_banList.insert(fd);
}

void Channel::deleteBan(int fd)
{
	this->_banList.erase(fd);
}

void Channel::addInvite(int fd)
{
	this->_inviteList.insert(fd);
}

void Channel::deleteInvite(int fd)
{
	this->_inviteList.erase(fd);
}
