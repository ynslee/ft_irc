#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"

/**
 * @brief 
 *  Syntax : PRIVMSG <target> <text to be sent>
 * 
 *  we are only hanlding nickname or channel. Here, channel syntax should be '#<channel>.
 *  PRIVMSG is used to send private messages between users, as well as to send messages to channels. 
 * 	
 * 
 *  Numeric replies:
 *   ERR_NOSUCHNICK (401)
 *   ERR_NOSUCHSERVER (402)
 *   ERR_NOSUCHCHANNEL (403)
 *   ERR_CANNOTSENDTOCHAN (404)
 *   ERR_NORECIPIENT (411)
 *   ERR_NOTEXTTOSEND (412)
 *   ERR_TOOMANYTARGETS (407)
 *   RPL_PRIVMSG
 * 
 *   yoonslee1!~yoonslee@194.136.126.51 PRIVMSG #hello :hello
 */

int cmdPrivmsg(Message &msg, Client *Client, std::map<std::string, Channel*> &channels)
{
	if (msg.params.size() == 0)
	{
		//send ERR_NORECIPIENT
	}
	if (msg.p
}