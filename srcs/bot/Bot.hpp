#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <sstream> //stoi(+11) 대신 istringstream쓰려고..
# include <ctime> // 
using namespace std;

class Bot {
	private:
		int				_curDate;
		int				_birthdate;
		vector<string>	_luckList;
		vector<string>	_itemList;

	public:
		Bot(); // list 초기화
		~Bot(); // list 삭제

		string	inputBirthdate(string param); // 생년월일 입력받음
		string	showTodayLuck() const;
		string	showLuckyItem() const;
};

#endif
