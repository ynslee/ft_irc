
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief used at the beginning of a connection to specify the username and realname of a new user
 * SYNTAX : USER <username> 0 * <realname>
 *
 * ERR_NEEDMOREPARAMS (461)
 * ERR_ALREADYREGISTERED (462)
 *
 * <username> needs to be at least 1 character long.
 */
int cmdUser(Message &msg, Client *Client)
{
	std::string servername = Client->getServerName();

	if (Client->getRegisteration() == 0)
	{
		send(Client->getClientFd(), ERR_NOTREGISTERED(servername).c_str(), ERR_NOTREGISTERED(servername).length(), 0);
		return (-1);
	}
	else if (Client->getRegisteration() == 3)
	{
		send(Client->getClientFd(), ERR_ALREADYREGISTRED(servername).c_str(), ERR_ALREADYREGISTRED(servername).length(), 0);
		return (-1);
	}
	else if (msg.params.size() != 3)
	{
		send(Client->getClientFd(), ERR_NEEDMOREPARAMS(servername).c_str(), ERR_NEEDMOREPARAMS(servername).length(), 0);
		return (-1);
	}
	else
	{
		Client->setUserName(msg.params[0]);
		if (msg.trailing.empty() == true)
		{
			Client->setRealName('~' + msg.params[0]);
		}
		else
			Client->setRealName(msg.trailing);
		int registeration = Client->getRegisteration();
		Client->setRegisteration(registeration + 1);
		return (0);
	}
}
