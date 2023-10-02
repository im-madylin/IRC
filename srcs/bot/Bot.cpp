#include "Bot.hpp"

Bot::Bot() {
	string str;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	_curDate = (t->tm_year + 1900) * 10000 + (t->tm_mon + 1) * 100 + t->tm_mday;

	ifstream fLuck;
	fLuck.open("todayLuck");
	/*
	if (!fLuck) {
		cout << "file \'todayLuck\' open fail" << endl;
	}
	*/
	while (getline(fLuck, str)) {
		_luckList.push_back(str);
	}
	fLuck.close();
	
	ifstream fItem;
	fItem.open("luckyItem");
	/*
	if (!fItem) {
		cout << "file \'luckyItem\' open fail" << endl; // 더 해야되나?
	}
	*/
	while (!getline(fItem, str)) {
		_luckList.push_back(str);
	}
	fItem.close();

	// init
	_birthdate = 0;
}

Bot::~Bot() {
	_luckList.clear();
	vector<string>().swap(_luckList); // capacity 0?
	_itemList.clear();
	vector<string>().swap(_itemList); // capacity 0
}

string Bot::inputBirthdate(string param) {
	int		intParam;

	if (param.length() != 8) {
		return ("format must be YYYYMMDD\n");
	}
	
	istringstream(param) >> intParam;
	if (intParam < 0 || intParam > _curDate) {
		return ("wrong birthdate\n");
	}
	return ("birth date has been saved\n");
}

string Bot::showTodayLuck() const {
	string reply;

	if (_birthdate == 0)
		reply = "you must input birthdate\n";
	else
		reply = "Your Today Luck : ";
	return reply;
}

string Bot::showLuckyItem() const {
	string reply;

	if (_birthdate == 0)
		reply = "you must input birthdate\n";
	else
		reply = "Your Lucky Item : ";
	return reply;
}