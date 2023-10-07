#include "Bot.hpp"

Bot::Bot() {
	string str;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	_curDate = (t->tm_year + 1900) * 10000 + (t->tm_mon + 1) * 100 + t->tm_mday;
	_birthDate = 0;
	initList();
}

Bot::~Bot() {
	_todayLuckColor.clear();
	vector<string>().swap(_luckyItemList); // capacity 0?
	_luckyItemList.clear();
	vector<string>().swap(_luckyItemList); // capacity 0
}

void Bot::initList() {
	_todayLuckColor.push_back("RED");
	_todayLuckColor.push_back("ORANGE");
	_todayLuckColor.push_back("YELLOW");
	_todayLuckColor.push_back("GREEN");
	_todayLuckColor.push_back("BLUE");
	_todayLuckColor.push_back("PURPLE");
	_todayLuckColor.push_back("WHITE");
	_todayLuckColor.push_back("BLACK");

	_luckyItemList.push_back("WALLET");
	_luckyItemList.push_back("CANDY");
	_luckyItemList.push_back("NECKLACE");
	_luckyItemList.push_back("SOCKS");
	_luckyItemList.push_back("SHIRT");
	_luckyItemList.push_back("PEN");
}

const string Bot::getRandom(const vector<string>& list) const {
	// 오늘 날짜와 생년월일을 결합한 값을 시드로 사용
	srand(static_cast<unsigned>(_curDate + _birthDate));
	return list[rand() % list.size()];
}

string Bot::inputBirthDate(string param) {
	int		intParam;

	if (param.length() != 8)
		return "ERROR : Format must be YYYYMMDD";

	for(unsigned int i = 0; i < param.length(); i++)
		if (param[i] < '0' || param[i] > '9')
			return "ERROR : Format must be YYYYMMDD";

	istringstream(param) >> intParam;
	if (intParam < 0 || intParam > _curDate)
		return "ERROR : Wrong birthdate";

	_birthDate = intParam;
	return "";
}

const string Bot::showLuckyItem() const {
	if (_birthDate == 0)
		return "ERROR : You must input birthdate";
	return " *****  YOUR LUCKY ITEM : " + getRandom(_todayLuckColor) + " " + getRandom(_luckyItemList) + " ***** ";
}
