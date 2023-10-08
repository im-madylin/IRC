#include "Bot.hpp"

Bot::Bot() : _birthDate(0) {
	string str;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	this->_curDate = (t->tm_year + 1900) * 10000 + (t->tm_mon + 1) * 100 + t->tm_mday;
	initList();
}

Bot::~Bot() {
	this->_todayLuckColor.clear();
	vector<string>().swap(this->_luckyItemList); // capacity 0?
	this->_luckyItemList.clear();
	vector<string>().swap(this->_luckyItemList); // capacity 0
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

const string Bot::getRandom(const vector<string>& list) const {
	// 오늘 날짜와 생년월일을 결합한 값을 시드로 사용
	srand(static_cast<unsigned>(this->_curDate + this->_birthDate));
	return list[rand() % list.size()];
}

string Bot::inputBirthDate(string param) {
	int intParam;

	if (param.length() != 8)
		return "ERROR : Format must be YYYYMMDD";

	for(unsigned int i = 0; i < param.length(); i++)
		if (param[i] < '0' || param[i] > '9')
			return "ERROR : Format must be YYYYMMDD";

	istringstream(param) >> intParam;
	if (intParam < 0 || intParam > this->_curDate)
		return "ERROR : Wrong birthdate";

	this->_birthDate = intParam;
	return "";
}

const string Bot::showLuckyItem() const {
	if (this->_birthDate == 0)
		return "ERROR : You must input birthdate";
	return " *****  YOUR LUCKY ITEM : " + getRandom(this->_todayLuckColor) + " " + getRandom(this->_luckyItemList) + " ***** ";
}
