
#include "../includes/Server.hpp"

void Server::closeClient(int i, int fd, Client *client)
{
    std::string channelname;
    std::string nickname;
    std::map<std::string, Client *>	clientlist;
    bool found = false;

    std::map<std::string, Channel*>::iterator it;
    for(it=_channels.begin(); it!=_channels.end(); it++)
    {
        clientlist = it->second->getClientList();
        // if (clientlist.size() == 0)
        //     continue;
        // else
        // {
            std::map<std::string, Client*>::iterator it2;
            for (it2=clientlist.begin(); it2!=clientlist.end(); it2++)
            {
                if (it2->second->getNickName() == client->getNickName())
                {
                    found = true;
                    break;
                }
            }
            if (found == true)
                it->second->removeFromChannel(client->getNickName());
        // }
    }
	this->_pfds[i] = this->_pfds[this->_pollfdCount - 1];
	// we have to remove from the client when we have it
	this->_pollfdCount--;
	_clients.erase(fd);
	close(fd);
    delete(client);
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

void Server::removeClientfromPollAndMap(int fd)
{
    std::map<int, Client*>::iterator it = _clients.find(fd);
    if (it != _clients.end())
    {
        delete it->second;
        _clients.erase(it);
    }
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