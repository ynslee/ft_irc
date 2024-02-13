#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "Common.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Message; 
class Server;
class Client;

int cmd_nick(Message &msg, int client_fd);
int cmd_pass(Message &msg, int client_fd);
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