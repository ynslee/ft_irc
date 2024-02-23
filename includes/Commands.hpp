#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Common.hpp"
# include "Message.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Client;
class Message;
class Channel;

void cmdCap(Message &msg, Client *Client);
int cmdNick(Message &msg, Client *Client, std::vector<std::string> &nick_names);
int cmdPass(Message &msg, Client *Client, std::string password);
void cmdQuit(Message &msg, Client *Client, std::map<std::string, Channel*> &channels,  std::vector<std::string> &nick_names);
int cmdUser(Message &msg, Client *Client);
int cmdWhois(Message &msg, Client *Client);
int cmdKill(Message &msg, Client *Client);
int cmdPrivmsg(Message &msg, Client *Client);


/*channel commands*/

int cmdJoin(Message &msg, Client *Client, std::map<std::string, Channel*> &channels);
int cmdKick(Message &msg, Client *Client);
int cmdInvite(Message &msg, Client *Client);
int cmdTopic(Message &msg, Client *Client);
// int cmdMode(Message &msg, Client *Client);
int cmdMotd(Message &msg, Client *Client);
int cmdOper(Message &msg, Client *Client);

void welcomeUser(Client *Client);

#endif
