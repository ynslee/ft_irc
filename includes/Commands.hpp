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
int cmd_quit(Message &msg, int client_fd);
int cmd_user(Message &msg, int client_fd);
int cmd_whois(Message &msg, int client_fd);
int cmd_kill(Message &msg, int client_fd);
int cmd_privmsg(Message &msg, int client_fd);


/*channel commands*/

int cmd_join(Message &msg, int client_fd);
int cmd_kick(Message &msg, int client_fd);
int cmd_invite(Message &msg, int client_fd);
int cmd_topic(Message &msg, int client_fd);
int cmd_mode(Message &msg, int client_fd);
int cmd_motd(Message &msg, int client_fd);
int cmd_oper(Message &msg, int client_fd);



#endif
