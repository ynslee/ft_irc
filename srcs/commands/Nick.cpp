#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

// Nick command----------------------------------------------
//Nicknames are non-empty strings with the following restrictions:

// They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A)
// question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
// They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
// They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
// They SHOULD NOT contain any dot character ('.', 0x2E).
// Servers MAY have additional implementation-specific nickname restrictions and SHOULD avoid the use of nicknames which are ambiguous
// with commands or command parameters where this could lead to confusion or error.


/**
 * @brief Nick command for giving the client a nickname or changing the previous one
 * Syntax: NICK <nickname>
 *
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	ERR_ERRONEUSNICKNAME (432)
 * 	ERR_NICKNAMEINUSE (433)
 * 	ERR_NICKCOLLISION (436)
 *
 * Example : /NICK nickname
 */

static void changeNickinChannels(Client *client, Server &server, std::string old_nick)
{
    std::vector<std::string>::iterator it;
    for(it = client->getChannelsJoined().begin(); it < client->getChannelsJoined().end(); it++)
    {
        std::string channel_name = *it;
        std::map<std::string, Channel*>::iterator channelIt = server.getChannels().find(channel_name);
        if(channelIt != server.getChannels().end())
        {
            if(channelIt->second->isOperator(old_nick) ==  true)
            {
                channelIt->second->removeOperator(old_nick);
                channelIt->second->addOperator(client->getNickName());
            }
            channelIt->second->getClientList().insert(std::make_pair(client->getNickName(), (client)));
            channelIt->second->getClientList().erase(old_nick);
        }
    }
    std::map<std::string, Channel*>::iterator iter2;
    for(iter2 = server.getChannels().begin(); iter2 != server.getChannels().end(); iter2++)
    {
        std::vector<std::string>::iterator inviteIter = std::find(iter2->second->getInvitedList().begin(), iter2->second->getInvitedList().end(), old_nick);
        if(inviteIter != iter2->second->getInvitedList().end())
        {
            iter2->second->getInvitedList().erase(inviteIter);
            iter2->second->getInvitedList().push_back(client->getNickName());
        }
    }
}

int isValidnick(std::string new_nick)
{
    if(!std::isalnum(new_nick[0]))
        return (-1);

    std::string::iterator it;
    for(it = new_nick.begin(); it != new_nick.end(); it++)
    {

        if(!std::isalnum(*it) && *it != '[' && *it != ']' && *it != '{' && *it != '}' && *it != '\\' && *it != '|' && *it != '_' && *it != '-')
            return(-1);
    }
    return(0);
}

int cmdNick(Message &msg, Client *Client, std::vector<std::string> &nick_names, Server &server)
{
    std::string hostname = Client->getHostName();
    std::string servername = Client->getServerName();

	if (Client->getCorrectPass() == false)
	{
		send(Client->getClientFd(), ERR_RESTRICTED(servername).c_str(), ERR_RESTRICTED(servername).length(), 0);
		return (-1);
	}
    if(Client->getRegisteration() == 0)
    {
        send(Client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
        return(-1);
    }
    if(msg.params.size() < 1)
    {
        send(Client->getClientFd(), ERR_NONICKNAMEGIVEN(hostname).c_str(), ERR_NONICKNAMEGIVEN(hostname).length(), 0);
        return(-1);
    }
    std::string new_nick = msg.params[0];
    if(isValidnick(new_nick))
    {
        send(Client->getClientFd(), ERR_ERRONEUSNICKNAME(hostname, new_nick).c_str(), ERR_ERRONEUSNICKNAME(hostname, new_nick).length(), 0);
        return(-1);
    }
    if(std::find(nick_names.begin(), nick_names.end(), new_nick) != nick_names.end())
    {
        send(Client->getClientFd(), ERR_NICKNAMEINUSE(hostname, new_nick).c_str(), ERR_NICKNAMEINUSE(hostname, new_nick).length(), 0);
        return(0);
    }
    if((Client->getRegisteration() == 3 || Client->getRegisteration() == 2 || Client->getRegisteration() == 1) && Client->getNickName().empty() == false)
    {
        std::string old_nick = Client->getNickName();
        Client->setNickName(new_nick);
        std::vector<std::string>::iterator it = std::find(nick_names.begin(),nick_names.end(), old_nick);
        nick_names.erase(it);
        nick_names.push_back(new_nick);
		changeNickinChannels(Client, server, old_nick);
        send(Client->getClientFd(), NICK_REPLY(old_nick,Client->getUserName(),Client->getIPaddress(), new_nick).c_str(), NICK_REPLY(old_nick,Client->getUserName(),Client->getIPaddress(), new_nick).length(),0);
        return(0);
    }
    Client->setNickName(new_nick);
    nick_names.push_back(new_nick);
    int registered = Client->getRegisteration();
    Client->setRegisteration(registered + 1);
    if (Client->getRegisteration() == 3 && Client->getWelcomeSent() == 0)
        welcomeUser(Client);
    return(0);
}
