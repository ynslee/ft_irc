#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief The INVITE command  is used to invite a user to a channel. The parameter <nickname> is the nickname of the person to be invited to the
 * target channel <channel>.
 * Syntax: INVITE <nickname> <channel>
 *
 * When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer,
 * and an INVITE message, with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.
 *
 *
 * Example :    INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar
 */

static void sendInviteMsg(std::string message, std::map<int, Client*> &clients, std::string invited_nick)
{
    std::map<int, Client *>::iterator it;
    for(it = clients.begin(); it != clients.end() ; ++it)
    {
        if(it->second->getNickName() == invited_nick)
        {
            // it->second->setSendbuf(message);
            send(it->second->getClientFd(), message.c_str(), message.length(), 0);
            return ;
        }
    }
}

int cmdInvite(Message &msg, Client *client,  std::map<std::string, Channel*> &channels,  std::vector<std::string> &nick_names, std::map<int, Client*> &clients)
{
    std::string hostname = client->getHostName();
    if (client->getWelcomeSent() != 1)
	{
		send(client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return (-1);
	}
    if(msg.params.size() < 2)
    {
        send(client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
        return(-1);
    }
    std::map<std::string, Channel*>::iterator channelIt = channels.find(msg.params[1]);
    if(channelIt == channels.end())
    {
        send(client->getClientFd(), ERR_NOSUCHCHANNEL(msg.params[1]).c_str(), ERR_NOSUCHCHANNEL(msg.params[1]).length(), 0);
        return(-1);
    }
    std::vector<std::string>::iterator it = std::find(client->getChannelsJoined().begin(), client->getChannelsJoined().end(), msg.params[1]);
    if(it == client->getChannelsJoined().end())
    {
        send(client->getClientFd(), ERR_NOTONCHANNEL(hostname,msg.params[1]).c_str(), ERR_NOTONCHANNEL(hostname,msg.params[1]).length(), 0);
        return(-1);
    }
    if((channelIt->second->getMode().find('i') != std::string::npos) && !channelIt->second->isOperator(client->getNickName()))
    {
        send(client->getClientFd(), ERR_CHANOPRIVSNEEDED(msg.params[1]).c_str(), ERR_CHANOPRIVSNEEDED(msg.params[1]).length(), 0);
        return(-1);
    }
    if(std::find(nick_names.begin(), nick_names.end(), msg.params[0]) == nick_names.end())
    {
        send(client->getClientFd(), ERR_NOSUCHNICK(msg.params[0]).c_str(), ERR_NOSUCHNICK(msg.params[0]).length(), 0);
        return(-1);
    }
    std::map<std::string, Client*>::iterator userChannelIt = channelIt->second->getClientList().find(msg.params[0]);
    if(userChannelIt != channelIt->second->getClientList().end())
    {
        send(client->getClientFd(), ERR_USERONCHANNEL(hostname,msg.params[0],msg.params[1]).c_str(), ERR_USERONCHANNEL(hostname,msg.params[0],msg.params[1]).length(), 0);
        return(-1);
    }
    std::vector<std::string>::iterator InviteIt = std::find(channelIt->second->getInvitedList().begin(), channelIt->second->getInvitedList().end(), msg.params[0]);
    if(InviteIt != channelIt->second->getInvitedList().end())
        return (0);
    channelIt->second->getInvitedList().push_back(msg.params[0]);
    send(client->getClientFd(),RPL_INVITING(hostname,client->getNickName(),msg.params[0],msg.params[1]).c_str(), RPL_INVITING(hostname,client->getNickName(),msg.params[0],msg.params[1]).length(), 0);
    std::string invite_message;
    invite_message = INVITE_MESSAGE(USER(client->getNickName(),client->getUserName(),client->getIPaddress()), msg.params[0], msg.params[1]);
    sendInviteMsg(invite_message,clients, msg.params[0]);
    return(0);
}
