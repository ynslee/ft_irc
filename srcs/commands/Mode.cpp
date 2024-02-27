#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Common.hpp"

int cmdMode(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	if (msg.params[0][0] == '#') //if mode command is used on a channel
	{
		std::map<std::string, Channel*>::iterator it;
		for (it = channels.begin(); it != channels.end(); it++)
		{
			// std::cout << "msg.params[0]: " << msg.params[0] << std::endl;
			// std::cout << "it->first: " << it->first << std::endl;
			std::string channelName = msg.params[0];
			if (it->first == channelName)
			{
				std::string clientNick = Client->getNickName();
				// std::cout << "clientNick: " << clientNick << std::endl;
				if (it->second->isOperator(clientNick) == true)
				{
					// std::cout << "msg.params[1]" << msg.params[1] << std::endl;
					it->second->setMode(msg.params[1], Client);
					// std::cout << "apply changes to channel mode!" << std::endl; // continue here!!
					return (0);
				}
				else
				{
					std::cout << "Coming in else" << std::endl;
					send(Client->getClientFd(), ERR_CHANOPRIVSNEEDED(Client->getUserName(), channelName).c_str(), ERR_CHANOPRIVSNEEDED(Client->getNickName(), channelName).length(), 0);
					break ;
				}
			}
			else
			{
				send(Client->getClientFd(), ERR_NOSUCHCHANNEL(Client->getUserName(), channelName).c_str(), ERR_NOSUCHCHANNEL(Client->getNickName(), channelName).length(), 0);
				return (-1);
			}
		}
	}
	return (0); // if command does not have # before channel name ignore command
}
