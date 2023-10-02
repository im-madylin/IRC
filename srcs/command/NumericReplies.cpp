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
	return ":" + prefix + " " + msg + "\r\n";
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
	vector<string> userList = channel.getUserList();
	for (vector<string>::iterator it = userList.begin(); it != userList.end(); it++)
		names += (*it) + " ";
	return "353 " + client + getSymbol(channel.getMode()) + channel.getChannelName() + " :" + names;
}

string ERR_NOSUCHCHANNEL(string client, string channel)
{
	return "403 " + client + " " + channel + " :No such channel";
}

string ERR_TOOMANYCHANNELS(string client, string channel)
{
	return "405 " + client + " " + channel + " :You have joined too many channels";
}

// TODO: ERROR(431): 만 나오고 msg가 나오지 않음
string ERR_NONICKNAMEGIVEN()
{
	return "431 :No nickname given";
}

// TODO: msg는 나오지만 nickname이 나오지 않음
string ERR_ERRONEUSNICKNAME(string nick)
{
	return "432 " + nick + " :Erroneus nickname";
}

// TODO: msg는 나오지만 nickname이 나오지 않음
string ERR_NICKNAMEINUSE(string nick)
{
	return "433 " + nick + " :Nickname is already in use";
}

// TODO: msg는 나오지만 nickname이 나오지 않음
string ERR_NICKCOLLISION(string nick)
{
	return "436 " + nick + " :Nickname collision KILL";
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
