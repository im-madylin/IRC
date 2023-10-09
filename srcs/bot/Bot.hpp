#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream> 
# include <cstdlib> 

using namespace std;

class Bot {
	private:
		int				_curDate;
		int				_birthDate;
		vector<string>	_todayLuckColor;
		vector<string> 	_luckyItemList;

	public:
		Bot();
		~Bot();

		const string	getRandom(const vector<string>& list) const;
		
		void			initList();
		string			inputBirthDate(string param);
		const string	showLuckyItem() const;
};

#endif
