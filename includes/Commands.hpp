#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Common.hpp"


int cmd_nick(std::string message, int client_fd);
int cmd_pass(std::string message, int client_fd);
int cmd_quit(std::string message, int client_fd);
int cmd_user(std::string message, int client_fd);
int cmd_whois(std::string message, int client_fd);
int cmd_kill(std::string message, int client_fd);
int cmd_privmsg(std::string message, int client_fd);


/*channel commands*/

int cmd_join(std::string message, int client_fd);
int cmd_kick(std::string message, int client_fd);
int cmd_invite(std::string message, int client_fd);
int cmd_topic(std::string message, int client_fd);
int cmd_mode(std::string message, int client_fd);
int cmd_motd(std::string message, int client_fd);
int cmd_oper(std::string message, int client_fd);



#endif