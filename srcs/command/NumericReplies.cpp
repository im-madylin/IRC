#include "NumericReplies.hpp"
#include "../user/User.hpp"
#include "../channel/Channel.hpp"

string getSymbol(string mode)
{
	if (mode == "+s")
		return " @ ";
	else if (mode == "+p")
		return " * ";
	return " = ";
}

string generateReply(string prefix, string msg)
{
	return ":" + prefix + " " + msg;
}

// :eunbi!root@127.0.0.1 TOPIC #channel :bibibi -> 이거는 TOPIC 하고 broadcast

// :irc.local 332 hello #bigbang :topic
string RPL_TOPIC(string client, Channel &channel)
{
	return "332 " + client + " " + channel.getChannelName() + " :" + channel.getTopic();
}

// :irc.local 353 test1 = #channel :@eunbi hello test1
string RPL_NAMREPLY(string client, Channel &channel)
{
	string names = "";
	for (vector<string>::iterator it = channel.getUserList().begin(); it != channel.getUserList().end(); it++)
		names += (*it) + " ";
	return "353 " + client + getSymbol(channel.getMode()) + channel.getChannelName() + " :" + names;
}

string ERR_NOSUCHCHANNEL(string client, string channel)
{
	return "403 " + client + " " + channel + " :No such channel";
}

string ERR_TOOMANYCHANNELS(string client, string channel)
{
	return "405 " + channel + " :You have joined too many channels";
}

// :irc.local 461 test22 JOIN :Not enough parameters.
string ERR_NEEDMOREPARAMS(string client, string command)
{
	return "461 " + client + " " + command + " :Not enough parameters";
}

string ERR_CHANNELISFULL(string client, string channel)
{
	return "471 " + client + " " + channel + " :Cannot join channel (+l)";
}

string ERR_INVITEONLYCHAN(string client, string channel)
{
	return "473 " + client + " " + channel + " :Cannot join channel (+i)";
}

string ERR_BANNEDFROMCHAN(string client, string channel)
{
	return "474 " + client + " " + channel + " :Cannot join channel (+b)";
}

string ERR_BADCHANNELKEY(string client, string channel)
{
	return "475 " + client + " " + channel + " :Cannot join channel (+k)";
}

// :irc.local 476 test22 babo :Bad Channel Mask
string ERR_BADCHANMASK(string client, string channel)
{
	return "476 " + client + " " + channel + " :Bad Channel Mask";
}
