#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

void welcomeUser(Client *Client)
{
	std::string userIP = Client->getIPaddress();
	if (Client->getRegisteration() == 3 && Client->getWelcomeSent() == 0)
	{
		Client->setSendbuf(RPL_WELCOME(hostname, username, Client->getServerName(), nick, userIP));
		
	}
}