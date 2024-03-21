
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
        std::map<std::string, Client*>::iterator it2;
        for (it2=clientlist.begin(); it2!=clientlist.end(); it2++)
        {
            if (it2->second->getNickName() == client->getNickName())
            {
                found = true;
                clientlist.erase(it2);
                break;
            }
        }
        if (found == true)
            it->second->removeFromChannel(client->getNickName());
    }
    std::vector<std::string>::iterator it4 = std::find(_nicknames.begin(), _nicknames.end(), client->getNickName());
    if (it4 != _nicknames.end())
        _nicknames.erase(it4);
	this->_pollfdCount--;
	_clients.erase(fd);
    _pfds.erase(_pfds.begin() + i);
	close(fd);
    delete(client);
    client = NULL;
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
        it->second = NULL;
        _clients.erase(it);
    }
    else
        return ;
    int i = 0;
    for(std::vector<pollfd>::iterator it = _pfds.begin(); i < this->_pollfdCount && it!= _pfds.end(); i++, it++)
	{				
        if(this->_pfds[i].fd == fd)
        {
            _pfds.erase(it);
            _pollfdCount--;
            break ;
        }
    }
    return ;
}