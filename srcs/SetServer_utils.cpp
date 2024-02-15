
#include "../includes/Server.hpp"

void Server::closeClient(int i, int fd)
{
	close(fd);
	this->_pfds[i] = this->_pfds[this->_pollfdCount - 1];
	// we have to remove from the client when we have it
	this->_pollfdCount--;
	_clients.erase(fd);
}


int Server::getCommandType(std::string command)
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
