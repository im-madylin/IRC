#include "Command.hpp"

/* ----------------------------------- PUBLIC ---------------------------------- */

Command::Command(Server *server) : _server(server) {
	this->_commands["PASS"] = &Command::PASS;
	this->_commands["PING"] = &Command::PING;
	this->_commands["NICK"] = &Command::NICK;
	this->_commands["USER"] = &Command::USER;
	this->_commands["JOIN"] = &Command::JOIN;
	this->_commands["PART"] = &Command::PART;
	this->_commands["TOPIC"] = &Command::TOPIC;
	this->_commands["PRIVMSG"] = &Command::PRIVMSG;
	this->_commands["NOTICE"] = &Command::NOTICE;
	this->_commands["KICK"] = &Command::KICK;
	this->_commands["INVITE"] = &Command::INVITE;
	this->_commands["QUIT"] = &Command::QUIT;
	this->_commands["WHO"] = &Command::WHO;
	this->_commands["MODE"] = &Command::MODE;
}

Command::~Command() {}

void Command::handleCommand(Message &message, User *user) {
	string command = message.getCommand();

	if (beforeRegisterCmd(command, user)) {
		if (this->_commands.find(command) != this->_commands.end()) {
			(this->*_commands[command])(message, user);
		}
	}
	else
		cout << "You must get Auth" << endl;
}

void Command::broadcast(int ignoreFd, Channel *channel, string message) {
	map<int, User *> user = channel->getUsers();
	for(map<int, User *>::iterator it = user.begin(); it != user.end(); it++) {
		if (it->first == ignoreFd)
			continue;
		it->second->appendMessage(message);
	}
}

void Command::broadcast(Channel *channel, string message) {
	map<int, User *> user = channel->getUsers();
	for(map<int, User *>::iterator it = user.begin(); it != user.end(); it++) {
		it->second->appendMessage(message);
	}
}

/* ---------------------------------- PRIVATE ---------------------------------- */

void Command::welcomMessage(User *user) {
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "Welcome to the")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), " _              _____                    ")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "(_)            |  ___|                   ")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "  _  _ __   ___ | |__    __ _  ___  _   _ ")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "| || '__| / __||  __|  / _` |/ __|| | | |")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "| || |   | (__ | |___ | (_| |\\__ \\| |_| |")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "|_||_|    \\___|\\____/  \\__,_||___/ \\__, |")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "                                    __/ |")));
	user->appendMessage(generateReply(this->_server->getServerPrefix(), RPL_MOTD(user->getNickname(), "                                   |___/ ")));
}

bool Command::beforeRegisterCmd(string cmd, User *user) {
	if (user->getIsRegistered() == true)
		return true;
	if (cmd == "PASS" || cmd == "QUIT")
		return true;
	if ((cmd == "NICK" || cmd == "USER") && user->getAuth() == true)
		return true;
	return false;
}
