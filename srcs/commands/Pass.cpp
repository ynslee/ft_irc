
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
int cmdPass(Message &msg, Client *Client, std::string password)
{
	std::cout << "CMINFG TO PASS" << std::endl;
	std::string hostname = Client->getHostName();
	std::string servername = Client->getServerName();

	if (msg.params.size() == 0)
	{
		send(Client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
		return (-1);
	}
	else if(Client->getRegisteration() == 3)
	{
		send(Client->getClientFd(), ERR_ALREADYREGISTRED(hostname).c_str(), ERR_ALREADYREGISTRED(hostname).length(), 0);
		return (-1);
	}
	else if (msg.params[0] == password)
	{
		std::cout << "IM HERE BISH" << std::endl;
		// Client->setSendbuf("Password correct!\r\n"); // from server to client
		Client->setRegisteration(1);
		return (0);
	}
	else
	{
		std::cout << "THIS WHERE I SHOULD BE" << std::endl;
		send(Client->getClientFd(), ERR_PASSWDMISMATCH(hostname).c_str(), ERR_PASSWDMISMATCH(hostname).length(), 0);
		// send(Client->getClientFd(), ERR_PASSWDMISMATCH(), strlen(ERR_PASSWDMISMATCH()), 0);
		return (-1);
	}

}
