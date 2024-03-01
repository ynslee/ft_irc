#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

int cmdPing(Message &msg, Client *Client)
{
	std::string hostname = Client->getHostName();
	std::string servername = Client->getServerName();
	
	std::string message = ":" + hostname + " PONG " + servername + " :" + msg.params[0] + "\r\n";
	send(Client->getClientFd(), message.c_str(), message.length(), 0);
	return (0);
}