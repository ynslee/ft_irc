#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Common.hpp"
# include "Message.hpp"
# include "Client.hpp"

class Client;
class Message;

void cmdCap(Message &msg, Client *Client);
int cmdNick(Message &msg, int client_fd);
int cmdPass(Message &msg, Client *Client, std::string password);
int cmdQuit(Message &msg, int client_fd);
int cmdUser(Message &msg, int client_fd);
int cmdWhois(Message &msg, int client_fd);
int cmdKill(Message &msg, int client_fd);
int cmdPrivmsg(Message &msg, int client_fd);


/*channel commands*/

int cmdJoin(Message &msg, int client_fd);
int cmdKick(Message &msg, int client_fd);
int cmdInvite(Message &msg, int client_fd);
int cmdTopic(Message &msg, int client_fd);
int cmdMode(Message &msg, int client_fd);
int cmdMotd(Message &msg, int client_fd);
int cmdOper(Message &msg, int client_fd);



#endif
