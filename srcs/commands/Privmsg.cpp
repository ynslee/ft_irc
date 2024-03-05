
#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"

/**
 * @brief
 *  Syntax : PRIVMSG <target> <text to be sent>
 *
 *  we are only hanlding nickname or channel. Here, channel syntax should be '#<channel>.
 *  PRIVMSG is used to send private messages between users, as well as to send messages to channels.
 *
 *
 *  Numeric replies:
 *   ERR_NOSUCHNICK (401)
 *   ERR_NOSUCHSERVER (402)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_CANNOTSENDTOCHAN (404)
 *   ERR_NORECIPIENT (411)
 *   ERR_NOTEXTTOSEND (412)
 *   ERR_TOOMANYTARGETS (407)
 *   RPL_PRIVMSG
 *
 *   yoonslee1!~yoonslee@194.136.126.51 PRIVMSG #hello :hello
 */

void messageToChannelClients(Message &msg, std::string nickname, std::map<std::string, Client*>_clientList)
{
	std::map<std::string, Client*>::iterator it;
	for (it=_clientList.begin(); it!=_clientList.end(); it++)
	{
		if (it->second->getNickName() != nickname)
		{
			std::string message = RPL_PRIVMSG(USER(nickname, it->second->getUserName(), it->second->getIPaddress()), msg.params[0], msg.trailing);
			std::cout << "message is: "	<< message << std::endl;
			send(it->second->getClientFd(), message.c_str(), message.length(), 0);
		}
	}
}

static int privmsgChannel(Message &msg, Client *client, std::map<std::string, Channel*> &channels)
{
	std::string channelName = msg.params[0];
	std::string hostname = client->getHostName();
	std::string nickname = client->getNickName();
	std::string username = client->getUserName();
	std::string text = msg.trailing;
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if(it->second->getChannelName() == channelName)
		{
			messageToChannelClients(msg, nickname, channels[channelName]->getClientList());
			return (0);
		}
	}
	send(client->getClientFd(), ERR_NOSUCHCHANNEL(channelName).c_str(), ERR_NOSUCHCHANNEL(channelName).length(), 0);
	return (-1);
}

static int privmsgClient(Message &msg, Client *client, std::map<int, Client*> &clients)
{
	std::string nickname = msg.params[0];
	std::string hostname = client->getHostName();
	std::string username = client->getUserName();
	std::string text = msg.trailing;

	std::map<int, Client*>::iterator it;
	for (it=clients.begin(); it!=clients.end(); it++)
	{
		if(it->second->getNickName() == nickname)
		{
			send(it->second->getClientFd(), RPL_PRIVMSG(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), msg.params[0], text).c_str(), RPL_PRIVMSG(USER(client->getNickName(), client->getUserName(), client->getIPaddress()), msg.params[0], text).length(), 0);
			return (0);
		}
	}
	send(client->getClientFd(), ERR_NOSUCHNICK(nickname).c_str(), ERR_NOSUCHNICK(nickname).length(), 0);
	return (-1);
}

static int privmsgServer(Message &msg, Client *client, std::map<int, Client*> &clients)
{
	std::string serverName = msg.params[0];
	std::string hostname = client->getHostName();
	std::string nickname = client->getNickName();
	std::string username = client->getUserName();
	std::string text = msg.trailing;

	std::map<int, Client*>::iterator it;
	for (it=clients.begin(); it!=clients.end(); it++)
	{
		if(it->second->getHostName() == hostname)
		{
			send(it->second->getClientFd(), RPL_PRIVMSG(USER(nickname, username, client->getIPaddress()), msg.params[0], msg.trailing).c_str(), RPL_PRIVMSG(USER(nickname, username, client->getIPaddress()), msg.params[0], msg.trailing).length(), 0);
			return (0);
		}
	}
	send(client->getClientFd(), ERR_NOSUCHSERVER(hostname, serverName).c_str(), ERR_NOSUCHSERVER(hostname, serverName).length(), 0);
	return (-1);
}

int cmdPrivmsg(Message &msg, Client *client, std::map<std::string, Channel*> &channels, std::map<int, Client*> &clients)
{
	if (msg.params.size() == 0)
	{
		send(client->getClientFd(), ERR_NORECIPIENT(client->getHostName()).c_str(), ERR_NORECIPIENT(client->getHostName()).length(), 0);
		return (-1);
	}
	if (msg.trailing.empty() == true)
	{
		send(client->getClientFd(), ERR_NOTEXTTOSEND(client->getHostName()).c_str(), ERR_NOTEXTTOSEND(client->getHostName()).length(), 0);
		return (-1);
	}
	if (msg.params.size() == 1 && msg.trailing.empty() == false)
	{
		if (msg.params[0].find(',') != std::string::npos)
		{
			send(client->getClientFd(), ERR_TOOMANYTARGETS(client->getHostName()).c_str(), ERR_TOOMANYTARGETS(client->getHostName()).length(), 0);
			return (-1);
		}
		else if (msg.params[0].find('.') != std::string::npos)
		{
			if (privmsgServer(msg, client, clients) == -1)
				return (-1);
		}
		else if (msg.params[0].find('.') == std::string::npos && msg.params[0][0] != '#')
		{
			if (isValidnick(msg.params[0]) == -1)
			{
				send(client->getClientFd(), ERR_NOSUCHNICK(client->getNickName()).c_str(), ERR_NOSUCHNICK(client->getNickName()).length(), 0);
				return (-1);
			}
			if (privmsgClient(msg, client, clients) == -1)
				return (-1);
		}
		else if (msg.params[0][0] == '#')
		{
			if (privmsgChannel(msg, client, channels) == -1 )
				return (-1);
		}
		else
			return (-1);
	}
	return (0);
}
