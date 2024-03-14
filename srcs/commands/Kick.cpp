#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief The KICK command can be used to request the forced removal of a user from a channel.
 * It causes the <user> to be removed from the <channel> by force.
 * Syntax: KICK <channel> <user> <comment>
 *
 * This message may be sent from a server to a client to notify the client that someone has been removed from a channel. 
 * In this case, the message <source> will be the client who sent the kick, and <channel> will be the channel which
 * the target client has been removed from.
 *
 *
 * Example :    KICK #Finnish Matthew ; Command to kick Matthew from #Finnish
 */

static void sendKickMsg(std::string message, Client *client, Client *client_kicked, Channel *channel)
{
    if(client == nullptr || client_kicked == nullptr)
        return ;
    std::map<std::string, Client *> clients_list = channel->getClientList();
    std::map<std::string, Client *>::const_iterator it;
    for(it = clients_list.begin(); it != clients_list.end() ; ++it)
    {
        if(client != NULL && it->first == client->getNickName())
            continue;
		// it->second->setSendbuf(message);
        send(it->second->getClientFd(), message.c_str(), message.length(), 0);
    }
    channel->removeFromChannel(client_kicked->getNickName());
    std::vector<std::string>::iterator iter = std::find(client_kicked->getChannelsJoined().begin(), client_kicked->getChannelsJoined().end(), channel->getChannelName());
    client_kicked->getChannelsJoined().erase(iter);
    std::vector<std::string>::iterator iter2 = std::find(channel->getInvitedList().begin(), channel->getInvitedList().end(), client_kicked->getNickName());
    std::cout << client_kicked->getNickName() << std::endl;
    if(iter2 != channel->getInvitedList().end())
        channel->getInvitedList().erase(iter2);
    client_kicked->unsetMaxChannels();
}

int cmdKick(Message &msg, Client *client,  std::map<std::string, Channel*> &channels)
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
    std::map<std::string, Channel*>::iterator channelIt = channels.find(msg.params[0]);
    if(channelIt == channels.end())
    {
        send(client->getClientFd(), ERR_NOSUCHCHANNEL(msg.params[0]).c_str(), ERR_NOSUCHCHANNEL(msg.params[0]).length(), 0);
        return(-1);
    }
    std::vector<std::string>::iterator it = std::find(client->getChannelsJoined().begin(), client->getChannelsJoined().end(), msg.params[0]);
    if(it == client->getChannelsJoined().end())
    {
        send(client->getClientFd(), ERR_NOTONCHANNEL(hostname,msg.params[0]).c_str(), ERR_NOTONCHANNEL(hostname,msg.params[0]).length(), 0);
        return(-1);
    }
    if(!channelIt->second->isOperator(client->getNickName()))
    {
        send(client->getClientFd(), ERR_CHANOPRIVSNEEDED(msg.params[0]).c_str(), ERR_CHANOPRIVSNEEDED(msg.params[0]).length(), 0);
        return(-1);
    }
    std::map<std::string, Client*>::iterator userChannelIt = channelIt->second->getClientList().find(msg.params[1]);
    if(userChannelIt == channelIt->second->getClientList().end())
    {
        send(client->getClientFd(), ERR_USERNOTINCHANNEL(hostname,msg.params[1],msg.params[0]).c_str(), ERR_USERNOTINCHANNEL(hostname,msg.params[1],msg.params[0]).length(), 0);
        return(-1);
    }
    std::string kick_message;
    kick_message = KICK_MESSAGE(USER(client->getNickName(),client->getUserName(),client->getIPaddress()), msg.params[0], userChannelIt->second->getNickName(), client->getNickName());
    if(msg.trailing.empty() == false)
        kick_message.append(" using " + msg.trailing + " as the reason \r\n");
    else
        kick_message.append("\r\n");
    sendKickMsg(kick_message,client,userChannelIt->second,channelIt->second);
    return(0);
}