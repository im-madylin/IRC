#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

# include <string>

# define ERR_NEEDMOREPARAMS 461
# define ERR_NEEDMOREPARAMS_MSG "Not enough parameters"
# define ERR_ALREADYREGISTRED 462
# define ERR_ALREADYREGISTRED_MSG "You may not reregister"

using namespace std;

class NumericReplies {
	private:
		
	public:
		NumericReplies();
		~NumericReplies();

		string generateReply(int num, string prefix);
};

# endif
