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

int cmdPrivmsg(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	if (msg.params.size() == 0)
	{
		send(Client->getClientFd(), ERR_NOSUCHNICK(Client->getHostName()).c_str(), ERR_NOSUCHNICK(Client->getHostName()).length(), 0);
	}
	if (msg.trailing.empty() == true)
	{
		//send ERR_NOTEXTTOSEND
	}
	if (msg.params.size() == 1 && msg.trailing.empty() == false)
	{
		if (msg.params[0].find('.') != std::string::npos)
			privmsgServer(msg, Client, channels);
		else if (msg.params[0].find('.') == std::string::npos && msg.params[0][0] != '#')
		{
			if (isValidnick(msg.params[0]) == -1)
			{
				send(Client->getClientFd(), ERR_NOSUCHNICK(Client->getNickName()).c_str(), ERR_NOSUCHNICK(Client->getNickName()).length(), 0);
			}
			privmsgClient(msg, Client, channels);
		}
		else if (msg.params[0][0] == '#')
			privmsgChannel(msg, Client, channels);
		else

	}
	return (0);
}