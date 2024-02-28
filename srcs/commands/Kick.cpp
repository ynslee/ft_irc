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

int cmdKick(Message &msg, Client *client,  std::map<std::string, Channel*> &channels)
{
    std::string hostname = client->getHostName();
    if(msg.params.size() < 2)
    {
        send(client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
            return(-1);
    }
    std::map<std::string, Channel*>::iterator channelIt = channels.find(msg.params[0]);
    if(channelIt == channels.end())
    {
        send(client->getClientFd(), ERR_NOSUCHCHANNEL(hostname,msg.params[0]).c_str(), ERR_NOSUCHCHANNEL(hostname,msg.params[0]).length(), 0);
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
        send(client->getClientFd(), ERR_CHANOPRIVSNEEDED(hostname,msg.params[0]).c_str(), ERR_CHANOPRIVSNEEDED(hostname,msg.params[0]).length(), 0);
            return(-1);
    }
    std::map<std::string, Client*>::iterator userChannelIt = channelIt->second->getClientList().find(msg.params[1]);
    if(userChannelIt == channelIt->second->getClientList().end())
    {
        send(client->getClientFd(), ERR_USERNOTINCHANNEL(hostname,msg.params[1],msg.params[0]).c_str(), ERR_USERNOTINCHANNEL(hostname,msg.params[1],msg.params[0]).length(), 0);
            return(-1);
    }
    std::string kick_message;
    kick_message = KICK_MESSAGE(client->getNickName(), msg.params[0]);
    if(!msg.params[2].empty())
        kick_message.append(" using" + msg.params[2] + "as the reason \r\n");
    else
        kick_message.append("\r\n");
    return(0);
}