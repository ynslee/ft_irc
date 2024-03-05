#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Common.hpp"

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
