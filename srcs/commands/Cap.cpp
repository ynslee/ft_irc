
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

void cmdCap(Message &msg, Client *Client)
{
	if (msg.params[0] == "LS" || msg.params[0] == "REQ" || msg.params[0] == "ACK" || msg.params[0] == "NAK")
	{
		std::string message = ":" + Client->getHostName() + " CAP * LS :" + "\r\n";
		send(Client->getClientFd(), message.c_str(), message.length(), 0);
		return ;
	}
	else if (msg.params[0] == "END")
		return ;
}
