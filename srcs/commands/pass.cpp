
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief PASS command for setting 'connection password'
 * Syntax: PASS <password>
 *
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_ALREADYREGISTERED (462)
 * 	ERR_PASSWDMISMATCH (464)
 *
 * Example : /PASS secretpassword
 */
int cmd_pass(Message &msg, Client *Client, std::string password)
{
	std::string servername = Client->getServerName();

	if (msg.params.size() == 0)
	{
		send(Client->getClientFd(), ERR_NEEDMOREPARAMS(servername).c_str(), ERR_NEEDMOREPARAMS(servername).length(), 0);
		return (-1);
	}
	else if(Client->getRegisteration() == 3)
	{
		send(Client->getClientFd(), ERR_ALREADYREGISTRED(servername).c_str(), ERR_ALREADYREGISTRED(servername).length(), 0);
		return (-1);
	}
	else if (msg.params[0] == password)
	{
		Client->setSendbuf("Password correct!");
		// std::cout << "***** correct password *****" << std::endl;
		Client->setRegisteration(1);
		return (0);
	}
	else
	{
		send(Client->getClientFd(), ERR_PASSWDMISMATCH(servername).c_str(), ERR_PASSWDMISMATCH(servername).length(), 0);
		return (-1);
	}

}
