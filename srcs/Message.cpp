#include "Message.hpp"
#include <iostream>

Message::Message() {}

Message::Message(const string &message) {
	parseMessage(message);
}

Message::~Message() {
}

string Message::getPrefix() {
	return this->_prefix;
}

string Message::getCommand() {
	return this->_command;
}

vector<string> Message::getParams() {
	return this->_params;
}

size_t Message::getParamsSize() {
	return this->_params.size();
}

void Message::setPrefix(string prefix) {
	this->_prefix = prefix;
}

void Message::setCommand(string command) {
	this->_command = command;
}

void Message::setParams(vector<string> params) {
	this->_params = params;
}

void Message::parseMessage(const string &message) {
	std::string prefix;
	std::string command;
	std::vector<std::string> params;

	size_t pos = 0;
	if (message[0] == ':') {
		pos = message.find(' ');
		prefix = message.substr(1, pos - 1);
	}

	size_t start = pos;
	pos = message.find(' ', start);
	command = message.substr(start, pos - start);

	while (pos != std::string::npos) {
		start = pos + 1;
		if (message[start] == ':') {
			if (message.substr(start + 1).length() > 0)
				params.push_back(message.substr(start + 1));
			break;
		}
		pos = message.find(' ', start);
		params.push_back(message.substr(start, pos - start));
	}
	setPrefix(prefix);
	setCommand(command);
	setParams(params);
}
