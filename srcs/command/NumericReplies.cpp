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

string RPL_AWAY(string client, string nickname, string message)
{
	return "301 " + client + " " + nickname + " :" + message;
}

string RPL_NOTOPIC(string client, string channel)
{
	return "331 " + client + " " + channel + " :No topic is set";
}

// :irc.local 332 hello #bigbang :topic
string RPL_TOPIC(string client, Channel &channel)
{
	return "332 " + client + " " + channel.getChannelName() + " :" + channel.getTopic();
}

// :irc.local 341 eunbi part :#invite
string RPL_INVITING(string client, string nickname, string channel)
{
	return "341 " + client + " " + nickname + " :" + channel;
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

string ERR_NOSUCHNICK(string client, string nickname)
{
	return "401 " + client + " " + nickname + " :No such nick/channel";
}

string ERR_NOSUCHCHANNEL(string client, string channel)
{
	return "403 " + client + " " + channel + " :No such channel";
}

string ERR_CANNOTSENDTOCHAN(string client, string channel)
{
	return "404 " + client + " " + channel + " :Cannot sent to channel";
}

string ERR_TOOMANYCHANNELS(string client, string channel)
{
	return "405 " + client + " " + channel + " :You have joined too many channels";
}

string ERR_TOOMANYTARGETS(string client, string target)
{
	return "407 " + client + " " + target + " :Duplicate recipients. No message delivered";
}

string ERR_NORECIPIENT(string client, string command)
{
	return "411 " + client + " :No recipient given " + command;
}

string ERR_NOTEXTTOSEND(string client)
{
	return "412 " + client + " :No text to send";
}

// TODO: ERROR(431): 만 나오고 msg가 나오지 않음, 닉네임이 주어지지 않았을때 사용하는 에러문구 특성상, client에 빈칸이 들어가서 안나오는 것으로 추측
string ERR_NONICKNAMEGIVEN(string client)
{
	return "431 " + client + " :No nickname given";
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

string ERR_USERNOTINCHANNEL(string client, string user, string channel)
{
	return "441 " + client + " " + user + " " + channel + " :They aren't on that channel";
}

// :irc.local 442 part #bang :You're not on that channel
string ERR_NOTONCHANNEL(string client, string channel)
{
	return "442 " + client + " " + channel + " :You're not on that channel";
}

string ERR_USERONCHANNEL(string client, string user, string channel)
{
	return "443 " + client + " " + user + " " + channel + " :is already on channel";
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

// :irc.local 473 part #invite :Cannot join channel (invite only)
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