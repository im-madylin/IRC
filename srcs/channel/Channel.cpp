#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string& name) : _name(name) {
	this->_topic = "";
	this->_key = "";
	this->_limit = MAX_CHANNEL_USER;
	addMode(CHANNEL_MODE_S);
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

set<ChannelMode> Channel::getModes()
{
	return this->_modes;
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

string Channel::getModeString()
{
	string mode = "+";
	for (set<ChannelMode>::iterator it = this->_modes.begin(); it != this->_modes.end(); it++) {
		switch (*it) {
			case CHANNEL_MODE_I:
				break ;
			case CHANNEL_MODE_T:
				mode += "t"; break ;
			case CHANNEL_MODE_K:
				mode += "k"; break ;
			case CHANNEL_MODE_O:
				break ;
			case CHANNEL_MODE_L:
				mode += "l"; break ;
			case CHANNEL_MODE_B:
				break ;
			case CHANNEL_MODE_S:
				mode += "s"; break ;
			case CHANNEL_MODE_N:
				mode += "n"; break ;
			default:
				break;
		}
	}
	return mode;
}

size_t Channel::getLimit()
{
	return this->_limit;
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

void Channel::setLimit(int limit)
{
	this->_limit = limit;
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
	if (_users.size() >= this->_limit)
		return true;
	return false;
}

bool Channel::hasMode(ChannelMode mode) const
{
	if (_modes.find(mode) != _modes.end())
		return true;
	return false;
}

void Channel::addMode(ChannelMode mode)
{
	this->_modes.insert(mode);
}

void Channel::deleteMode(ChannelMode mode)
{
	if (_modes.find(mode) == _modes.end())
		return ;
	_modes.erase(mode);
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
