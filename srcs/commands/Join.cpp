#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"

/**
 * @brief Indicates that the client wants to join the given channel(s), each channel using the given key for it.
 * 	The server receiving the command checks whether or not the client can join the given channel,
 * 	and processes the request.
 *
 * 	While a client is joined to a channel, they receive all relevant information about
 * 	that channel including who are in the channel and the commands that could be used in the channel.
 * 	They receive all PRIVMSG and when someone joins to the channel, and they also
 * 	receive QUIT messages from other clients joined to the same channel (to let them
 * 	know those users have left the channel and the network).
 *
 *
 *  Examples:
 * 	[CLIENT]  JOIN #foobar
 *  [SERVER] ; join channel #foobar.
 *
 * 	[CLIENT]  JOIN #foo foobar
 * 	[SERVER]; join channel #foo using key "foobar".
 */

static bool validChannelName(std::string channel)
{
	std::size_t found = channel.find('#');

	if (channel.size() < 50 && found == 0)
	{
		if (channel.find(',') == std::string::npos && channel.find(':') == std::string::npos)
			return true;
	}
	return false;
}

static void topicMessage(Channel *channel, Client *client)
{
	if (channel->getTopic().empty() == false)
		client->addSendbuf(RPL_TOPIC(client->getHostName(), client->getNickName(), channel->getChannelName(), channel->getTopic()));
	else
		client->addSendbuf(RPL_NOTOPIC(client->getHostName(), client->getNickName(), channel->getChannelName()));
}

static void successfulJoinMessage(Client *client, std::string channelName, Channel *channel, std::string nick)
{
	std::string nickList;
	std::map<std::string, Client*> &clientList = channel->getClientList();
	std::string prefixNick;
	std::string clientNick;

	if(channel->isOperator(nick) == true)
		nickList = "@" + nick;
	else
		nickList = nick;
	clientNick = nickList;
	std::map<std::string, Client*>::iterator it;
	if (clientList.empty() == false)
	{
		for (it=clientList.begin(); it!=clientList.end(); it++)
		{
			if (it->first.compare(nick) != 0)
			{
				if (channel->isOperator(it->second->getNickName()) == true)
					prefixNick = "@" + it->second->getNickName();
				else
					prefixNick = it->second->getNickName();
				send(it->second->getClientFd(), RPL_JOIN(USER(clientNick, client->getUserName(), client->getIPaddress()), channelName, client->getRealName()).c_str(), RPL_JOIN(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), channelName, client->getRealName()).length(), 0);
				nickList += " " + prefixNick;
			}
		}
	}
	client->addSendbuf(RPL_NAMREPLY(client->getHostName(), client->getUserName(), channelName, nickList));
	client->addSendbuf(RPL_ENDOFNAMES(client->getHostName(), client->getUserName(), channelName));
}

static void printJoinMessage(Client *client, std::string channelName)
{
	std::cout << MAGENTA << client->getNickName() << RESET << " [" << MAGENTA << client->getUserName() << "@" << client->getIPaddress() << RESET << "] has joined " << BOLD << channelName << RESET << std::endl;
}

static bool checkIfClientExists(std::map<std::string, Client*> &clientList, std::string nick)
{
	std::map<std::string, Client*>::iterator it;
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
			client->setMaxChannels();
			client->setNewChannel(channelName);
			client->setSendbuf(RPL_JOIN(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), channelName, client->getRealName()));
			if(channels[channelName]->getClientList().size() == 0)
				channels[channelName]->addOperator(client->getNickName());
			it->second->addToChannel(*client);
			topicMessage(channels[channelName], client);
			successfulJoinMessage(client, channelName, channels[channelName], client->getNickName());
			printJoinMessage(client, channelName);
			return (0);
		}
	}
	channels.insert(std::make_pair(channelName, new Channel(channelName)));
	if(channels[channelName]->getClientList().size() == 0)
		channels[channelName]->addOperator(client->getNickName());
	channels[channelName]->addToChannel(*client);
	client->setMaxChannels();
	client->setNewChannel(channelName);
	client->setSendbuf(RPL_JOIN(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), channelName, client->getRealName()));
	topicMessage(channels[channelName], client);
	successfulJoinMessage(client, channelName, channels[channelName], client->getNickName());
	printJoinMessage(client, channelName);
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
				client->setMaxChannels();
				client->setNewChannel(channelName);
				client->setSendbuf(RPL_JOIN(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), channelName, client->getRealName()));
				if(channels[channelName]->getClientList().size() == 0)
					channels[channelName]->addOperator(client->getNickName());
				it->second->addToChannel(*client);
				topicMessage(channels[channelName], client);
				successfulJoinMessage(client, channelName, channels[channelName], client->getNickName());
				printJoinMessage(client, channelName);
				return (0);
			}
			else
			{
				send(client->getClientFd(), ERR_BADCHANNELKEY(client->getHostName(), channelName).c_str(), ERR_BADCHANNELKEY(client->getHostName(), channelName).length(), 0);
				return (-1);
			}
		}
	}
	channels.insert(std::make_pair(channelName, new Channel(channelName)));
	if(channels[channelName]->getClientList().size() == 0)
		channels[channelName]->addOperator(client->getNickName());
	channels[channelName]->addToChannel(*client);
	channels[channelName]->setChannelKey(key);
	client->setMaxChannels();
	client->setNewChannel(channelName);
	client->setSendbuf(RPL_JOIN(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), channelName, client->getRealName()));
	topicMessage(channels[channelName], client);
	successfulJoinMessage(client, channelName, channels[channelName], client->getNickName());
	printJoinMessage(client, channelName);
	return (0);
}

static int clientAlreadyInChannel(std::map<std::string, Channel*> &channels, Client *client)
{
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		std::string channelName = it->first;
		std::vector<std::string> channelList = client->getChannelsJoined();
		std::vector<std::string>::iterator it2;
		for(it2=channelList.begin(); it2!=channelList.end(); ++it2)
		{
			if (*it2 == channelName)
				return (1);
		}
	}
	return (0);
}

static int clientErrorChecks(Client *client, std::map<std::string, Channel*> &channels, std::string channelName)
{
	std::string hostname = client->getHostName();
	bool invited = false;
	std::vector <std::string> invitedClients;

	if (channelName.empty())
	{
		send(client->getClientFd(), "Wrong channel Name. Write such as ex) #chat", 44, 0);
		return (-1);
	}
	if(client->getRegisteration() <= 2)
	{
		send(client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return(-1);
	}
	if (client->getMaxChannels() == 3)
	{
		if (clientAlreadyInChannel(channels, client) == 1)
			return (-1);
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
				invitedClients = it->second->getInvitedList();
				std::vector<std::string>::iterator it2;
				for (it2=invitedClients.begin(); it2!=invitedClients.end(); it2++)
				{
					if (*it2 == client->getNickName())
						invited = true;
				}
				if (invited == false)
				{
					send(client->getClientFd(), ERR_INVITEONLYCHAN(client->getUserName(), channelName).c_str(), ERR_INVITEONLYCHAN(client->getUserName(), channelName).length(), 0);
					return (-1);
				}
			}
			}
			else if (it->second->getClientList().size() == static_cast<size_t>(it->second->getUserLimit()))
			{
				send(client->getClientFd(), ERR_CHANNELISFULL(client->getUserName(), channelName).c_str(), ERR_CHANNELISFULL(client->getUserName(), channelName).length(), 0);
				return (-1);
			}
		}
	return (0);
}

void printClientList(std::map<std::string, Channel*> &channels)
{
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		std::cout << "channel name is " << it->first << std::endl;
		std::map<std::string, Client*>::iterator it2;
		for (it2=it->second->getClientList().begin(); it2!=it->second->getClientList().end(); it2++)
			std::cout << "client is " << it2->second->getNickName() << std::endl;
	}
	return;
}


int cmdJoin(Message &msg, Client *client, std::map<std::string, Channel*> &channels)
{
	std::string channelName;
	std::string hostname = client->getHostName();

	if (client->getWelcomeSent() != 1)
	{
		send(client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return (-1);
	}
	if (msg.trailing.size() > 0)
		return (-1);	
	if (msg.params.size() == 0 && msg.trailing.empty() == true)
	{
		if (msg.trailing_flag == 0)
			send(client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
		return (-1);
	}
	if (msg.params[0].size() == 1 && msg.params[0].compare("#") == 0)
	{
		send(client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
		return (-1);
	}
	if (validChannelName(msg.params[0]) == false)
		return (-1);
	channelName = msg.params[0];
	if (clientErrorChecks(client, channels, channelName) == -1)
		return (-1);
	if (channels.size() == 0)
	{
		if(msg.params.size() <= 2)
		{
			channels.insert(std::make_pair(channelName, new Channel(channelName)));
			if(channels[channelName]->getClientList().size() == 0)
				channels[channelName]->addOperator(client->getNickName());
			channels[channelName]->addToChannel(*client);
			if (msg.params.size() == 2)
				channels[channelName]->setChannelKey(msg.params[1]);
			client->setSendbuf(RPL_JOIN(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), channelName, client->getRealName()));
			client->setMaxChannels();
			client->setNewChannel(channelName);
			topicMessage(channels[channelName], client);
			successfulJoinMessage(client, channelName, channels[channelName], client->getNickName());
			printJoinMessage(client, channelName);
			return (0);
		}
		else
			return (-1);
	}
	if (channels.size() > 0 && msg.params.size() == 1)
		return (joinExistingServerWithoutKey(channels, channelName, client));
	if (channels.size() > 0 && msg.params.size() == 2)
		return (joinExistingServerWithKey(channels, channelName, msg.params[1], client));
	return (0);
}