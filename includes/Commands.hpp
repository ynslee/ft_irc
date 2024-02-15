#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Common.hpp"
# include "Message.hpp"
# include "Client.hpp"

class Client;
class Message;

void cmd_cap(Message &msg, Client *Client);
int cmd_nick(Message &msg, int client_fd);
int cmd_pass(Message &msg, Client *Client, std::string password);
int cmd_quit(Message &msg, Client *Client);
int cmd_user(Message &msg, Client *Client);
int cmd_whois(Message &msg, Client *Client);
int cmd_kill(Message &msg, Client *Client);
int cmd_privmsg(Message &msg, Client *Client);


/*channel commands*/

int cmd_join(Message &msg, Client *Client);
int cmd_kick(Message &msg, Client *Client);
int cmd_invite(Message &msg, Client *Client);
int cmd_topic(Message &msg, Client *Client);
int cmd_mode(Message &msg, Client *Client);
int cmd_motd(Message &msg, Client *Client);
int cmd_oper(Message &msg, Client *Client);



#endif
