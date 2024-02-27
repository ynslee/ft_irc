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
			if (it->first == msg.params[0])
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
					send(Client->getClientFd(), ERR_CHANOPRIVSNEEDED(Client->getUserName(), msg.params[0]).c_str(), ERR_CHANOPRIVSNEEDED(Client->getNickName(), msg.params[0]).length(), 0);
					return (-1);
				}
			}
		}
		send(Client->getClientFd(), ERR_NOSUCHCHANNEL(Client->getUserName(), msg.params[0]).c_str(), ERR_NOSUCHCHANNEL(Client->getNickName(), msg.params[0]).length(), 0);
		return (-1);
	}
	return (0); // if command does not have # before channel name ignore command
}
/*
if (target is this->client)
{
	if (modestring)
		apply changes
	if (one or more unknown modestrings)
		send ERR_UMODEUNKNOWNFLAG (501)
	send mode message RPL_UMODEIS (221)
}
if (target is an existing channelname)
{
	if (modestring)
		if (client has priviledges)
			apply changes
		else
			ERR_CHANOPRIVSNEEDED (482)
	if (one or more unknown modestrings)
		send ERR_UMODEUNKNOWNFLAG (501)
	RPL_CHANNELMODEIS (324)
}
else
	ERR_NOSUCHNICK (401)
	ERR_NOSUCHCHANNEL (403)

*/
