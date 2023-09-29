#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

# define MAX_MESSAGE_SIZE 512

using namespace std;

class Message {
	private:
		string			_prefix;
		string			_command;
		vector<string> 	_params;

	public:
		Message();
		Message(const string &message);
		~Message();

		string			getPrefix();
		string			getCommand();
		vector<string>	getParams();

		void		setPrefix(string prefix);
		void		setCommand(string command);
		void		setParams(vector<string> params);

		void	parseMessage(const string &message);
};

#endif
