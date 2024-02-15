
#include "../includes/Server.hpp"

void Server::close_client(int i, int fd)
{
	close(fd);
	this->pfds[i] = this->pfds[this->pollfd_count - 1];
	// we have to remove from the client when we have it
	this->pollfd_count--;
	_clients.erase(fd);
}


int Server::get_command_type(std::string command)
{
    std::string commands[15] = 
    {
        "CAP",
        "PASS",
        "NICK",
        "USER",
        "JOIN",
        "INVITE",
        "KICK",
        "TOPIC",
        "MODE",
        "MODT",
        "PRIVMSG",
        "WHOIS",
        "KILL",
        "OPER"
    };

    for (int i = 0; i < 14; i++)
    {
        if (command == commands[i])
            return (i);
    }
    return (INVALID);

}
