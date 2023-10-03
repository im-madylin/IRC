#ifndef NUMERIC_REPLIES_HPP
# define NUMERIC_REPLIES_HPP

# include <string>
# include <vector>

using namespace std;

class Channel;
class User;

string getSymbol(string mode);
string generateReply(string prefix, string msg);

// 331
string RPL_NOTOPIC(string client, string channel);
// 332
string RPL_TOPIC(string client, Channel &channel);
// 353
string RPL_NAMREPLY(string client, Channel &channel);

// 403
string ERR_NOSUCHCHANNEL(string client, string channel);
// 405
string ERR_TOOMANYCHANNELS(string client, string channel);
// 431
string ERR_NONICKNAMEGIVEN();
// 432
string ERR_ERRONEUSNICKNAME(string nick);
// 433
string ERR_NICKNAMEINUSE(string nick);
// 436
string ERR_NICKCOLLISION(string nick);
// 442
string ERR_NOTONCHANNEL(string client, string channel);
// 461
string ERR_NEEDMOREPARAMS(string client, string command);
// 462
string ERR_ALREADYREGISTERED(string client);
// 471
string ERR_CHANNELISFULL(string client, string channel);
// 473
string ERR_INVITEONLYCHAN(string client, string channel);
// 474
string ERR_BANNEDFROMCHAN(string client, string channel);
// 475
string ERR_BADCHANNELKEY(string client, string channel);
// 476
string ERR_BADCHANMASK(string client, string channel);
// 482
string ERR_CHANOPRIVSNEEDED(string client, string channel);

# endif
