
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

void cmdCap(Message &msg, Client *Client)
{
	(void)Client;
	if (msg.params[0] == "LS" || msg.params[0] == "REQ" || msg.params[0] == "ACK" || msg.params[0] == "NAK" || msg.params[0] == "END")
		return ;
}
