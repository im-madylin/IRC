#include "NumericReplies.hpp"

NumericReplies::NumericReplies()
{
}

NumericReplies::~NumericReplies()
{
}

string NumericReplies::generateReply(int num, string prefix)
{
	switch (num)
	{
		case ERR_NEEDMOREPARAMS:
			return prefix + ":" + ERR_NEEDMOREPARAMS_MSG;
		case ERR_ALREADYREGISTRED:
			return ":" + (string)ERR_ALREADYREGISTRED_MSG;
		default:
			return "";
	}
}
