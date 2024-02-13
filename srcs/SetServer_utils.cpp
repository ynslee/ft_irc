
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
    if(command == "PASS")
        return(0);
    else if(command == "NICK")
        return(1);
    else if(command == "USER")
        return(2);
    else if(command == "JOIN")
        return(3);
    else if(command == "INVITE")
        return(4);
    else if(command == "KICK")
        return(5);
    else if(command == "TOPIC")
        return(6);
    else if(command == "MODE")
        return(7);
    else if(command == "MODT")
        return(8);
    else if(command == "PRIVMSG")
        return(9);
    else if(command == "WHOIS")
        return(10);
    else if(command == "KILL")
        return(11);
    else if(command == "OPER")
        return(12);
    else
        return(13);
}