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

// :eunbi!root@127.0.0.1 TOPIC #bang :hello2
string RPL_NOTOPIC(string client, string channel)
{
	return "331 " + client + " " + channel + " :No topic is set";
}

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

string RPL_YOUREOPER(string client)
{
	return "381 " + client + " " + ":You are now an IRC operator" ;
}

string ERR_NOSUCHCHANNEL(string client, string channel)
{
	return "403 " + client + " " + channel + " :No such channel";
}

string ERR_TOOMANYCHANNELS(string client, string channel)
{
	return "405 " + client + " " + channel + " :You have joined too many channels";
}

// TODO: ERROR(431): 만 나오고 msg가 나오지 않음, 닉네임이 주어지지 않았을때 사용하는 에러문구 특성상, client에 빈칸이 들어가서 안나오는 것으로 추측
string ERR_NONICKNAMEGIVEN(string client)
{
	return "431 " + client + + " " + ":No nickname given";
}

string ERR_ERRONEUSNICKNAME(string client)
{
	return "432 " + client + " " + client + " :Erroneus nickname";
}

string ERR_NICKNAMEINUSE(string client)
{
	return "433 " + client + " " + client + " :Nickname is already in use";
}

string ERR_NICKCOLLISION(string client)
{
	return "436 " + client + " " + client + " :Nickname collision KILL";
}

// :irc.local 442 part #bang :You're not on that channel
string ERR_NOTONCHANNEL(string client, string channel)
{
	return "442 " + client + " " + channel + " :You're not on that channel";

}

// :irc.local 461 test22 JOIN :Not enough parameters.
string ERR_NEEDMOREPARAMS(string client, string command)
{
	return "461 " + client + " " + command + " :Not enough parameters";
}

string ERR_ALREADYREGISTERED(string client)
{
	return "462 " + client + " " + ":You may not reregister";
}

string ERR_PASSWDMISMATCH(string client)
{
	return "464 " + client + " " + ":Password incorrect";
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

string ERR_CHANOPRIVSNEEDED(string client, string channel)
{
	return "482 " + client + " " + channel + " :You're not channel operator";
}

string ERR_NOOPERHOST(string client)
{
	return "491 " + client + " " + ":No O-lines for your host";
}