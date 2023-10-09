#include "Bot.hpp"

/* ----------------------------------- PUBLIC ---------------------------------- */

Bot::Bot() : _birthDate(0) {
	string str;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	this->_curDate = (t->tm_year + 1900) * 10000 + (t->tm_mon + 1) * 100 + t->tm_mday;
	initList();
}

Bot::~Bot() {
	this->_todayLuckColor.clear();
	this->_luckyItemList.clear();
}

const string Bot::getRandom(const vector<string>& list) const {
	srand(static_cast<unsigned>(this->_curDate + this->_birthDate));
	return list[rand() % list.size()];
}

void Bot::initList() {
	this->_todayLuckColor.push_back("RED");
	this->_todayLuckColor.push_back("ORANGE");
	this->_todayLuckColor.push_back("YELLOW");
	this->_todayLuckColor.push_back("GREEN");
	this->_todayLuckColor.push_back("BLUE");
	this->_todayLuckColor.push_back("PURPLE");
	this->_todayLuckColor.push_back("WHITE");
	this->_todayLuckColor.push_back("BLACK");

	this->_luckyItemList.push_back("WALLET");
	this->_luckyItemList.push_back("CANDY");
	this->_luckyItemList.push_back("NECKLACE");
	this->_luckyItemList.push_back("SOCKS");
	this->_luckyItemList.push_back("SHIRT");
	this->_luckyItemList.push_back("PEN");
}

string Bot::inputBirthDate(string param) {
	int intParam;
	int year;
	int month;
	int day;

	if (param.length() != 8)
		return "ERROR : Format must be YYYYMMDD";

	for(unsigned int i = 0; i < param.length(); i++)
		if (param[i] < '0' || param[i] > '9')
			return "ERROR : Format must be YYYYMMDD";

	istringstream(param) >> intParam;
	year = intParam/10000;
	month = (intParam % 10000) / 100;
	day = intParam % 100;
	if (intParam < 0 || intParam > this->_curDate || \
		year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31)
		return "ERROR : Wrong birthdate";

	this->_birthDate = intParam;
	return "";
}

const string Bot::showLuckyItem() const {
	if (this->_birthDate == 0)
		return "ERROR : You must input birthdate";
	return " *****  YOUR LUCKY ITEM : " + getRandom(this->_todayLuckColor) + " " + getRandom(this->_luckyItemList) + " ***** ";
}
