#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"

static std::string getChannelName(std::string channel)
{
	std::size_t found = channel.find('#');
	if (found != std::string::npos)
		return (channel);
	else
		return ("#" + channel);
}

static int joinExistingServerWithoutKey(std::map<std::string, Channel*> &channels, std::string channelName, Client *Client)
{
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if (it->first == channelName)
		{
			if (it->second->getChannelKey().empty() == false)
			{
				send(Client->getClientFd(), ERR_BADCHANNELKEY(Client->getHostName(), channelName).c_str(), ERR_BADCHANNELKEY(Client->getHostName(), channelName).length(), 0);
				return (-1);
			}
			it->second->addToChannel(*Client);
			return (0);
		}
	}
	channels[channelName] = new Channel(channelName);
	channels[channelName]->addToChannel(*Client);
	Client->setNewChannel(channelName);
	return (0);
}

static int joinExistingServerWithKey(std::map<std::string, Channel *> &channels, std::string channelName, std::string key, Client *Client)
{
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if (it->first == channelName)
		{
			if (it->second->getChannelKey() == key)
			{
				it->second->addToChannel(*Client);
				return (0);
			}
			else
			{
				send(Client->getClientFd(), ERR_BADCHANNELKEY(Client->getHostName(), channelName).c_str(), ERR_BADCHANNELKEY(Client->getHostName(), channelName).length(), 0);
				return (-1);
			}
		}
	}
	channels[channelName] = new Channel(channelName);
	channels[channelName]->addToChannel(*Client);
	channels[channelName]->setChannelKey(key);
	Client->setNewChannel(channelName);
	return (0);
}

int cmdJoin(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	std::string channelName;
	std::string hostname = Client->getHostName();

	if(Client->getRegisteration() <= 2)
    {
        send(Client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
        return(-1);
    }
	if (msg.params.size() == 0)
	{
		send(Client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
		return (-1);
	}
	if (channels.size() == 0) // no channels on the server yet
	{
		channelName = getChannelName(msg.params[0]);
		if(msg.params.size() <= 2)
		{
			channels[channelName] = new Channel(channelName);
			channels[channelName]->addToChannel(*Client);
			if (msg.params.size() == 2)
				channels[channelName]->setChannelKey(msg.params[1]);
			Client->setSendbuf(RPL_JOIN(USER(Client->getNickName(), Client->getUserName(), Client->getIPaddress()), channelName, Client->getRealName()));
			Client->setNewChannel(channelName);
			return (0);
		}
		else
			return (-1);
	}
	if (channels.size() > 0 && msg.params.size() == 1)
		return (joinExistingServerWithoutKey(channels, msg.params[0], Client));
	if (channels.size() > 0 && msg.params.size() == 2)
		return (joinExistingServerWithKey(channels, msg.params[0], msg.params[1], Client));
	return (0);
}
