#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

# include <string>
# include <vector>

# include "../user/User.hpp"
# include "../channel/Channel.hpp"
# include "../Utils.hpp"

using namespace std;

class Channel;
class User;

string getSymbol(Channel &channel);
string generateReply(string prefix, string msg);

// 315
string RPL_ENDOFWHO(string client, string name);
// 324
string RPL_CHANNELMODEIS(string client, string channel, string mode);
// 331
string RPL_NOTOPIC(string client, string channel);
// 332
string RPL_TOPIC(string client, Channel &channel);
// 341
string RPL_INVITING(string client, string nickname, string channel);
// 352
string RPL_WHOREPLY(string client, string chname, string servname, User &user);
// 353
string RPL_NAMREPLY(string client, Channel &channel);
// 366
string RPL_ENDOFNAMES(string client, string channel);
// 372
string RPL_MOTD(string client, string msg);
// 381
string RPL_YOUREOPER(string client);
// 401
string ERR_NOSUCHNICK(string client, string nickname);
// 403
string ERR_NOSUCHCHANNEL(string client, string channel);
// 404
string ERR_CANNOTSENDTOCHAN(string client, string channel);
// 405
string ERR_TOOMANYCHANNELS(string client, string channel);
// 407
string ERR_TOOMANYTARGETS(string client, string target);
// 411
string ERR_NORECIPIENT(string client, string command);
// 412
string ERR_NOTEXTTOSEND(string client);
// 431
string ERR_NONICKNAMEGIVEN(string client);
// 432
string ERR_ERRONEUSNICKNAME(string client);
// 433
string ERR_NICKNAMEINUSE(string client);
// 436
string ERR_NICKCOLLISION(string client);
// 441
string ERR_USERNOTINCHANNEL(string client, string user, string channel);
// 442
string ERR_NOTONCHANNEL(string client, string channel);
// 443
string ERR_USERONCHANNEL(string client, string user, string channel);
// 461
string ERR_NEEDMOREPARAMS(string client, string command);
// 462
string ERR_ALREADYREGISTERED(string client);
// 464
string ERR_PASSWDMISMATCH(string client);
// 467
string ERR_KEYSET(string client, string channel);
// 471
string ERR_CHANNELISFULL(string client, string channel);
// 472
string ERR_UNKNOWNMODE(string client, char mode, string channel);
// 473
string ERR_INVITEONLYCHAN(string client, string channel);
// 474
string ERR_BANNEDFROMCHAN(string client, string channel);
// 475
string ERR_BADCHANNELKEY(string client, string channel);
// 476
string ERR_BADCHANMASK(string client, string channel);
// 477
string ERR_NOCHANMODES(string client, string channel);
// 482
string ERR_CHANOPRIVSNEEDED(string client, string channel);
// 491
string ERR_NOOPERHOST(string client);

// 696
// "<client> <target chan/user> <mode char> <parameter> :<description>"
// *.freenode.net 696 eunbi #banana k * :You must specify a parameter for the key mode. Syntax: <key>.
// *.freenode.net 696 eunbi #banana o * :You must specify a parameter for the op mode. Syntax: <nick>.
// *.freenode.net 696 bbb #banana l * :You must specify a parameter for the limit mode. Syntax: <limit>.
string ERR_INVALIDMODEPARAM(string client, string channel, char mode, string parameter, string description);

// mode reply
string MODE_REPLY(string channel, string parameter);

# endif
