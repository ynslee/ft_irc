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

void broadcastToAllClients(std::map<std::string, Client*>_clientList, std::string message)
{
	std::map<std::string, Client*>::iterator it;
	for (it=_clientList.begin(); it!=_clientList.end(); it++)
	{
		send(it->second->getClientFd(), message.c_str(), message.length(), 0);
	}
}

static int privmsgChannel(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	std::string channelName = msg.params[0];
	std::string hostname = Client->getHostName();
	std::string nickname = Client->getNickName();
	std::string username = Client->getUserName();
	std::string text = msg.trailing;
	std::map<std::string, Channel*>::iterator it;
	for (it=channels.begin(); it!=channels.end(); it++)
	{
		if(it->second->getChannelName() == channelName)
		{
			broadcastToAllClients(channels[channelName]->getClientList(), RPL_PRIVMSG(USER(nickname, username, Client->getIPaddress()), msg.params[0], msg.trailing));
			return (0);
		}
	}
	send(Client->getClientFd(), ERR_NOSUCHCHANNEL(channelName).c_str(), ERR_NOSUCHCHANNEL(channelName).length(), 0);
	return (-1);
}

static int privmsgClient(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	std::string nickname = msg.params[0];
	std::string hostname = Client->getHostName();
	std::string username = Client->getUserName();
	std::string text = msg.trailing;
	std::map<std::string, Client*>::iterator it;
	for (it=Client->getServer()->getClientList().begin(); it!=Client->getServer()->getClientList().end(); it++)
	{
		if(it->second->getNickName() == nickname)
		{
			send(it->second->getClientFd(), RPL_PRIVMSG(USER(Client->getNickName(), Client->getUserName(), Client->getIPaddress()), msg.params[0], msg.trailing).c_str(), RPL_PRIVMSG(USER(Client->getNickName(), Client->getUserName(), Client->getIPaddress()), msg.params[0], msg.trailing).length(), 0);
			return (0);
		}
	}
	send(Client->getClientFd(), ERR_NOSUCHNICK(nickname).c_str(), ERR_NOSUCHNICK(nickname).length(), 0);
	return (-1);
}

int cmdPrivmsg(Message &msg, Client *Client, std::map<std::string, Channel*> &channels, std::map<int, Client*> &clients)
{
	if (msg.params.size() == 0)
	{
		send(Client->getClientFd(), ERR_NORECIPIENT(Client->getHostName()).c_str(), ERR_NORECIPIENT(Client->getHostName()).length(), 0);
	}
	if (msg.trailing.empty() == true)
	{
		send(Client->getClientFd(), ERR_NOTEXTTOSEND(Client->getHostName()).c_str(), ERR_NOTEXTTOSEND(Client->getHostName()).length(), 0);
	}
	if (msg.params.size() == 1 && msg.trailing.empty() == false)
	{
		if (msg.params[0].find(',') != std::string::npos)
		{
			send(Client->getClientFd(), ERR_TOOMANYTARGETS(Client->getHostName()).c_str(), ERR_TOOMANYTARGETS(Client->getHostName()).length(), 0);
			return (-1);
		}
		if (msg.params[0].find('.') != std::string::npos)
		{
			if (privmsgServer(msg, Client) == -1)
				return (-1);
		}
		else if (msg.params[0].find('.') == std::string::npos && msg.params[0][0] != '#')
		{
			if (isValidnick(msg.params[0]) == -1)
			{
				send(Client->getClientFd(), ERR_NOSUCHNICK(Client->getNickName()).c_str(), ERR_NOSUCHNICK(Client->getNickName()).length(), 0);
				return (-1);
			}
			if (privmsgClient(msg, Client, channels) == -1)
				return (-1);
		}
		else if (msg.params[0][0] == '#')
			privmsgChannel(msg, Client, channels);
		else
			return (-1);
	}
	return (0);
}