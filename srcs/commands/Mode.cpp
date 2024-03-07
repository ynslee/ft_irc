#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Common.hpp"

/**
 * @brief The MODE command is provided so that users may query and change the
characteristics of a channel.  For more details on available modes
and their uses, see "Internet Relay Chat: Channel Management" [IRC-
CHAN].  Note that there is a maximum limit of three (3) changes per
command for modes that take a parameter.

[CLIENT] MODE #Finnish +o Kilroy	//Command to give 'chanop' privileges to Kilroy on channel #Finnish.
[CLIENT] MODE #42 +k oulu			//Command to set the channel key to "oulu".
[CLIENT] MODE #eu-opers +l 10		//Command to set the limit for the number of users on channel "#eu-opers" to 10.
*/

void	changeUserLimit(Message &msg, Channel *channel)
{
	if (msg.params[1][0] == '+' && !msg.params[2].empty())
	{
		unsigned int temp = 0;
		try
		{
			temp = static_cast<unsigned int>(stoi(msg.params[2]));
		}
		catch (const std::invalid_argument& e)
		{
			return ;
		}
		catch (const std::out_of_range& e)
		{
			return ;
		}
		channel->setUserLimit(temp);
	}
	if (msg.params[1][0] == '-' && msg.params[2].empty())
	{
		channel->setUserLimit(10);
	}
}

int cmdMode(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	std::string hostname = Client->getHostName();

	if (Client->getWelcomeSent() != 1)
	{
		send(Client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return (-1);
	}
	if (msg.params[0][0] == '#' && msg.params.size() > 1)
	{
		std::map<std::string, Channel*>::iterator it;
		for (it = channels.begin(); it != channels.end(); it++)
		{
			std::string channelName = msg.params[0];
			if (it->first == channelName)
			{
				std::string clientNick = Client->getNickName();
				if (it->second->isOperator(clientNick) == true)
				{
					if (msg.params[1][1] == 'i' || msg.params[1][1] == 't')
					{
						it->second->setMode(msg.params[1], Client);
						return (0);
					}
					else if (msg.params[1][1] == 'k')
					{
						if (msg.params[1][0] == '+')
							it->second->setChannelKey(msg.params[2]);
						if (msg.params[1][0] == '-')
							it->second->setChannelKey("");
					}
					else if (msg.params[1][1] == 'o')
					{
						if (msg.params[1][0] == '+')
							it->second->addOperator(msg.params[2]);
						if (msg.params[1][0] == '-')
							it->second->removeOperator(msg.params[2]);
					}
					else if (msg.params[1][1] == 'l')
						changeUserLimit(msg, it->second);
				}
				else
				{
					send(Client->getClientFd(), ERR_CHANOPRIVSNEEDED(channelName).c_str(), ERR_CHANOPRIVSNEEDED(channelName).length(), 0);
					return (0);
				}
			}
			else
			{
				send(Client->getClientFd(), ERR_NOSUCHCHANNEL(channelName).c_str(), ERR_NOSUCHCHANNEL(channelName).length(), 0);
				return (-1);
			}
		}
	}
	return (0); // if command does not have # before channel name ignore command
}
