#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"

static std::string getChannelName(std::string channel, std::map<std::string, Channel*> &channels)
{
	std::size_t found = channel.find('#');
	std::string channelName;

	if (found != std::string::npos)
		channelName = channel;
	else
		channelName = "#" + channel;
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if (it->first == channelName)
			return (channelName);
	}
	return (channelName);
}

static bool checkIfClientExists(std::map<const std::string, Client*> &clientList, std::string nick)
{
	std::map<const std::string, Client*>::iterator it;
	for (it=clientList.begin(); it!=clientList.end(); it++)
	{
		if (it->first == nick)
			return (true);
	}
	return (false);
}

static int joinExistingServerWithoutKey(std::map<std::string, Channel*> &channels, std::string channelName, Client *client)
{
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if (it->first == channelName)
		{
			if (it->second->getChannelKey().empty() == false)
			{
				send(client->getClientFd(), ERR_BADCHANNELKEY(client->getHostName(), channelName).c_str(), ERR_BADCHANNELKEY(client->getHostName(), channelName).length(), 0);
				return (-1);
			}
			if (checkIfClientExists(it->second->getClientList(), client->getNickName()) == true)
				return (0);
			it->second->addToChannel(*client);
			return (0);
		}
	}
	channels.insert(std::make_pair(channelName, &Channel(channelName)));
	channels[channelName]->addToChannel(*client);
	client->setNewChannel(channelName);
	return (0);
}

static int joinExistingServerWithKey(std::map<std::string, Channel *> &channels, std::string channelName, std::string key, Client *client)
{
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if (it->first == channelName)
		{
			if (it->second->getChannelKey() == key)
			{
				if (checkIfClientExists(it->second->getClientList(), client->getNickName()) == true)
					return (0);
				it->second->addToChannel(*client);
				return (0);
			}
			else
			{
				send(client->getClientFd(), ERR_BADCHANNELKEY(client->getHostName(), channelName).c_str(), ERR_BADCHANNELKEY(client->getHostName(), channelName).length(), 0);
				return (-1);
			}
		}
	}
	channels[channelName]->addToChannel(*client);
	channels[channelName]->setChannelKey(key);
	client->setNewChannel(channelName);
	return (0);
}

static int clientErrorChecks(Client *client, std::map<std::string, Channel*> &channels, std::string channelName)
{
	std::string hostname = client->getHostName();

	if(client->getRegisteration() <= 2)
	{
		send(client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return(-1);
	}
	if (client->getMaxChannels() == 3)
	{
		send(client->getClientFd(), ERR_TOOMANYCHANNELS(client->getUserName(), channelName).c_str(), ERR_TOOMANYCHANNELS(client->getUserName(), channelName).length(), 0);
		return (-1);
	}
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if (it->first == channelName)
		{
			if (it->second->getMode().find('i') != std::string::npos)
			{
				send(client->getClientFd(), ERR_INVITEONLYCHAN(client->getUserName(), channelName).c_str(), ERR_INVITEONLYCHAN(client->getUserName(), channelName).length(), 0);
				return (-1);
			}
			else if (it->second->getUserLimit() == 10)
			{
				send(client->getClientFd(), ERR_CHANNELISFULL(client->getUserName(), channelName).c_str(), ERR_CHANNELISFULL(client->getUserName(), channelName).length(), 0);
				return (-1);
			}
		}
	}
	return (0);
}

int cmdJoin(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	std::string channelName;
	std::string hostname = Client->getHostName();

	if (msg.params.size() == 0)
	{
		send(Client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
		return (-1);
	}
	channelName = getChannelName(msg.params[0], channels);
	if(Client->getRegisteration() <= 2)
    {
        send(Client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
        return(-1);
    }
	if (Client->getMaxChannels() == 3)
	{
		send(Client->getClientFd(), ERR_TOOMANYCHANNELS(Client->getUserName(), channelName).c_str(), ERR_TOOMANYCHANNELS(Client->getUserName(), channelName).length(), 0);
		return (-1);
	}
	if (channels.size() == 0)
	{
		if(msg.params.size() <= 2)
		{
			channels.insert(std::make_pair(channelName, &Channel(channelName)));
			channels[channelName]->addToChannel(*Client);
			if (msg.params.size() == 2)
				channels[channelName]->setChannelKey(msg.params[1]);
			Client->setSendbuf(RPL_JOIN(USER(Client->getNickName(), Client->getUserName(), Client->getIPaddress()), channelName));
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