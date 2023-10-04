#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream> //stoi(+11) 대신 istringstream쓰려고..
# include <cstdlib> //

using namespace std;

class Bot {
	private:
		int				_curDate;
		int				_birthDate;
		vector<string>	_todayLuckList;
		vector<string> 	_luckyItemList;

	public:
		Bot(); // list 초기화
		~Bot(); // list 삭제

		void			initList();
		string			inputBirthDate(string param); // 생년월일 입력받음
		const string	getRandom(const vector<string>& list) const;
		const string	showTodayLuck() const;
		const string	showLuckyItem() const;
};

#endif
