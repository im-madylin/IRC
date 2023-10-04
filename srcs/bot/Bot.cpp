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
	_todayLuckList.clear();
	vector<string>().swap(_luckyItemList); // capacity 0?
	_luckyItemList.clear();
	vector<string>().swap(_luckyItemList); // capacity 0
}

void Bot::initList() {
	_todayLuckList.push_back("today's luck [1]");
	_todayLuckList.push_back("today's luck [2]");
	_todayLuckList.push_back("today's luck [3]");

	_luckyItemList.push_back("lucky item <1>");
	_luckyItemList.push_back("lucky item <2>");
	_luckyItemList.push_back("lucky item <3>");
}

const string Bot::getRandom(const vector<string>& list) const {
	// 오늘 날짜와 생년월일을 결합한 값을 시드로 사용
	srand(static_cast<unsigned>(_curDate + _birthDate));
	return list[rand() % list.size()];
}

string Bot::inputBirthDate(string param) {
	int		intParam;

	if (param.length() != 8) {
		return ("format must be YYYYMMDD");
	}
	
	istringstream(param) >> intParam;
	if (intParam < 0 || intParam > _curDate) {
		return ("wrong birthdate");
	}

	_birthDate = intParam;
	return ("birth date has been saved");
}

const string Bot::showTodayLuck() const {
	string reply;

	if (_birthDate == 0)
		reply = "you must input birthdate";
	else if (_todayLuckList.empty())
		reply = "list is empty";
	else
		reply = "Your Today Luck : " + getRandom(_todayLuckList);
	return reply;
}

const string Bot::showLuckyItem() const {
	string reply;

	if (_birthDate == 0)
		reply = "you must input birthdate";
	else if (_luckyItemList.empty())
		reply = "list is empy";
	else
		reply = "Your Lucky Item : " + getRandom(_luckyItemList);
	return reply;
}

/*
int main()
{
	Bot		bot;
	string	input;

	while (1)
	{
		cout << "birthdate : ";
		cin >> input;
		cout << bot.inputBirthDate(input) << endl;

		cout << bot.showTodayLuck() << endl;
		cout << bot.showLuckyItem() << endl;
	}
}
*/
