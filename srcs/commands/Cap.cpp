
#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

void cmd_cap(Message &msg, Client *Client)
{
	if (msg.params[0] == "LS" || msg.params[0] == "REQ" || msg.params[0] == "ACK" || msg.params[0] == "NAK")
	{
		Client->setSendbuf(":" + Client->getServerName() + " CAP * LS :" + "\r\n");
		std::cout << "sending>>> " << Client->getSendbuf() << std::endl;
	}
}