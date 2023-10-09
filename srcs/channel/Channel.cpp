#include "Channel.hpp"

/* ----------------------------------- PUBLIC ---------------------------------- */

Channel::Channel() {}

Channel::Channel(std::string& name) : _name(name) {
	this->_topic = "";
	this->_key = "";
	this->_limit = MAX_CHANNEL_USER;
	addMode(CHANNEL_MODE_S);
}

Channel::~Channel() {}

string Channel::getChannelName() const {
	return this->_name;
}

map<int, User *>Channel::getUsers() const {
	return this->_users;
}

vector<string> Channel::getUserList() {
	vector<string> list;

	for(map<int, User *>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++) {
		string name;
		if (isOperator(it->first))
			name += '@';
		name += it->second->getNickname(); 
		list.push_back(name);
	}
	return list;
}

string Channel::getTopic() const {
	return this->_topic;
}

set<ChannelMode> Channel::getModes() const {
	return this->_modes;
}

set<int> Channel::getInviteList() const {
	return this->_inviteList;
}

string Channel::getKey() const {
	return this->_key;
}

string Channel::getModeString() {
	string mode = "+";
	string params = "";
	for (set<ChannelMode>::iterator it = this->_modes.begin(); it != this->_modes.end(); it++) {
		switch (*it) {
			case CHANNEL_MODE_I:
				mode += "i"; break ;
			case CHANNEL_MODE_T:
				mode += "t"; break ;
			case CHANNEL_MODE_K: {
				mode += "k";
				params += this->_key + " ";
				break ;
			}
			case CHANNEL_MODE_O:
				break ;
			case CHANNEL_MODE_L: {
				mode += "l";
				params += toString(this->_limit) + " ";
				break ;
			}
			case CHANNEL_MODE_S:
				mode += "s"; break ;
			case CHANNEL_MODE_N:
				mode += "n"; break ;
			default:
				break;
		}
	}
	return " " + mode + " " + params;
}

size_t Channel::getLimit() const {
	return this->_limit;
}

void Channel::setTopic(string topic) {
	this->_topic = topic;
}

void Channel::setKey(string key) {
	this->_key = key;
}

void Channel::setLimit(int limit) {
	this->_limit = limit;
}

void Channel::addUser(int fd, User *user) {
	if (this->_users.empty())
		this->_operator.insert(fd);
	this->_users.insert(make_pair(fd, user));
}

void Channel::deleteUser(int fd) {
	map<int, User *>::iterator it;

	if (isOperator(fd))
		deleteOper(fd);
	
	it = this->_users.find(fd);
	if (it == this->_users.end())
		return ;
	this->_users.erase(fd);
}

User* Channel::findUser(int fd) const {
	map<int, User *>::const_iterator it;

	it = this->_users.find(fd);
	if (it == this->_users.end())
		return NULL;
	return it->second;
}

User* Channel::findUser(string nickname) const {
	map<int, User *>::const_iterator it;

	for(it = this->_users.begin(); it != this->_users.end(); it++)
		if (!it->second->getNickname().compare(nickname))
			return it->second;
	return it->second;
}

bool Channel::isOperator(int fd) const {
	if (this->_operator.find(fd) != this->_operator.end())
		return true;
	return false;
}

bool Channel::isOperator(User *user) const {
	for(set<int>::iterator it = this->_operator.begin(); it != this->_operator.end(); it++)
		if (*it == user->getFd())
			return true;
	return false;
}

void Channel::addOper(int fd) {
	this->_operator.insert(fd);
}

void Channel::deleteOper(int fd) {
	this->_operator.erase(fd);
}

bool Channel::isExistUser(int fd) const {
	if (this->_users.find(fd) == this->_users.end())
		return false;
	return true;
}

bool Channel::isFull() const {
	if (this->_users.size() >= this->_limit)
		return true;
	return false;
}

bool Channel::hasMode(ChannelMode mode) const {
	if (this->_modes.find(mode) != this->_modes.end())
		return true;
	return false;
}

void Channel::addMode(ChannelMode mode) {
	this->_modes.insert(mode);
}

void Channel::deleteMode(ChannelMode mode) {
	if (this->_modes.find(mode) == this->_modes.end())
		return ;
	this->_modes.erase(mode);
}

bool Channel::isInvited(int fd) const {
	if (this->_inviteList.find(fd) != this->_inviteList.end())
		return true;
	return false;
}

void Channel::addInvite(int fd) {
	this->_inviteList.insert(fd);
}

void Channel::deleteInvite(int fd) {
	this->_inviteList.erase(fd);
}

string Channel::executeBot(string date) {
	if (date.length() == 0)
		return "Please input your birthday format YYYYMMDD";

	string ret = this->_bot.inputBirthDate(date);
	if (ret.length() != 0)
		return ret;
	return this->_bot.showLuckyItem();
}
