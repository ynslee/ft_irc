#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Common.hpp"

int cmdMode(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	if (msg.params[0][0] == '#' && msg.params.size() > 1) //if mode command is used on a channel and by user
	{
		std::cout << "Tulin sisalle!!!" << std::endl;
		std::map<std::string, Channel*>::iterator it;
		for (it = channels.begin(); it != channels.end(); it++)
		{
			// std::cout << "msg.params[0]: " << msg.params[0] << std::endl;
			// std::cout << "it->first: " << it->first << std::endl;
			std::string channelName = msg.params[0];
			if (it->first == channelName) // looking for the right channel
			{
				std::string clientNick = Client->getNickName();
				// std::cout << "clientNick: " << clientNick << std::endl;
				if (it->second->isOperator(clientNick) == true) // checking if cient is operator
				{
					// std::cout << "msg.params[1]" << msg.params[1] << std::endl;
					it->second->setMode(msg.params[1], Client);
					// std::cout << "apply changes to channel mode!" << std::endl; // continue here!!
					return (0);
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
