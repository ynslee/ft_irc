#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include "../../includes/Common.hpp"

int cmdMode(Message &msg, Client *Client)
{
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
	std::cout << &msg << Client->getMode() << std::endl;
	return (0);
}
