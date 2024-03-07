#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Common.hpp"
# include "Message.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"

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
int cmdPrivmsg(Message &msg, Client *client, std::map<std::string, Channel*> &channels, std::map<int, Client*> &clients);
int cmdPing(Message &msg, Client *Client);

/*channel commands*/

int cmdJoin(Message &msg, Client *Client, std::map<std::string, Channel*> &channels);
int cmdKick(Message &msg, Client *Client,  std::map<std::string, Channel*> &channels);
int cmdInvite(Message &msg, Client *client,  std::map<std::string, Channel*> &channels, std::vector<std::string> &nick_names, std::map<int, Client*> &clients);
int cmdTopic(Message &msg, Client *client, std::map<std::string, Channel*> &channels);
int cmdMode(Message &msg, Client *Client, std::map<std::string, Channel*> &channels);
int cmdMotd(Message &msg, Client *Client);
int cmdOper(Message &msg, Client *Client);

void welcomeUser(Client *Client);
int isValidnick(std::string new_nick);

#endif
