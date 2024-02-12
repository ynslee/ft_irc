#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Common.hpp"


class Commands
{
	private:
		Commands();
        Commands(Commands const& other);
	    ~Commands();
	    Commands	&operator=(Commands const &other);

        std::string _command;
	public:
		Commands(std::string command);
		~Commands();
};

int cmd_nick(std::string message);
int cmd_pass(std::string message);
int cmd_quit(std::string message);
int cmd_user(std::string message);
int cmd_whois(std::string message);
int cmd_kill(std::string message);
int cmd_privmsg(std::string message);


/*channel commands*/

int cmd_join(std::string message);
int cmd_kick(std::string message);
int cmd_invite(std::string message);
int cmd_topic(std::string mesagge);
int cmd_mode(std::string message);
int cmd_motd(std::string message);
int cmd_oper(std::string message);



#endif