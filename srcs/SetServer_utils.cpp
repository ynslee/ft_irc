
#include "../includes/Server.hpp"

void Server::closeClient(int i, int fd)
{
    std::string channelname;
    std::string nickname;
    std::map<std::string, Client *>	clientlist;
    bool found = false;

	close(fd);
    std::map<int, Client*>::iterator it3;
    for(it3=_clients.begin(); it3!=_clients.end(); it3++)
    {
        if (it3->second->getClientFd() == fd)
        {
            nickname = it3->second->getNickName();
            break;
        }
    }
    if (nickname.empty() == false)
    {
        std::map<std::string, Channel*>::iterator it;
        for(it=_channels.begin(); it!=_channels.end(); it++)
        {
            clientlist = it->second->getClientList();
            std::map<std::string, Client*>::iterator it2;
            for (it2=clientlist.begin(); it2!=clientlist.end(); it2++)
            {
                if (it2->second->getNickName() == nickname)
                    found = true;
            }
            if (found == true)
                it->second->removeFromChannel(nickname);
        }
    }
	this->_pfds[i] = this->_pfds[this->_pollfdCount - 1];
	// we have to remove from the client when we have it
	this->_pollfdCount--;
	_clients.erase(fd);
}


int Server::getCommandType(std::string command)
{
    std::string commands[16] =
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
        "MOTD",
        "PRIVMSG",
        "WHOIS",
        "KILL",
        "OPER",
        "QUIT",
        "PING"
    };

    for (int i = 0; i < 16; i++)
    { 
        if (command == commands[i])
        {
            return (i);
        }
    }
    return (INVALID);

}

void Server::removeClientfromPoll(int fd)
{
    for(int i = 0; i < this->_pollfdCount; i++)
	{				
        if(this->_pfds[i].fd == fd)
        {
            this->_pfds[i] = this->_pfds[this->_pollfdCount - 1];
            _pollfdCount--;
            _clients.erase(fd);
            break ;
        }
    }
    return ;
}