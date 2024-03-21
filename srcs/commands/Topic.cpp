#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief The TOPIC command is used to change or view the topic of the given channel. 
 * If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one. 
 * If <topic> is an empty string, the topic for the channel will be cleared
 *
 * Syntax: TOPIC <channel> [<topic>]
 * 
 * This message may be sent from a server to a client to notify the client that someone has been removed from a channel. 
 * In this case, the message <source> will be the client who sent the kick, and <channel> will be the channel which
 * the target client has been removed from.
 *
 * If the topic of a channel is changed or cleared, every client in that channel (including the author of the topic change) will receive a 
 * TOPIC command with the new topic as argument (or an empty argument if the topic was cleared) alerting them to how the topic has changed
 *
 * Example :      TOPIC #test :New topic          ; Setting the topic on "#test" to "New topic".
 * 
 * TOPIC #test :                   ; Clearing the topic on "#test"
 * TOPIC #test                     ; Checking the topic for "#test"
 */


static void sendTopicMsg(std::string message, Channel *channel, Client *client, int flag)
{
    if(client == nullptr)
        return ;
    std::map<std::string, Client *> clients_list = channel->getClientList();
    std::map<std::string, Client *>::const_iterator it;
    if(flag == 1)
    {
        for(it = clients_list.begin(); it != clients_list.end() ; ++it)
        {
            if(client != NULL && it->first == client->getNickName())
                continue;
            send(it->second->getClientFd(), message.c_str(), message.length(), 0);
        }
    }
    send(client->getClientFd(), RPL_TOPIC(client->getHostName(), client->getNickName(), channel->getChannelName(), channel->getTopic()).c_str(), \
    RPL_TOPIC(client->getHostName(), client->getNickName(), 
    channel->getChannelName(), channel->getTopic()).length(), 0);
}

int cmdTopic(Message &msg, Client *client, std::map<std::string, Channel*> &channels)
{
    std::string hostname = client->getHostName();
    if (client->getWelcomeSent() != 1)
	{
		send(client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return (-1);
	}
    if(msg.params.size() < 1)
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
    if((channelIt->second->getMode().find('t') != std::string::npos) && !channelIt->second->isOperator(client->getNickName()) && msg.trailing_flag == 1)
    {
        send(client->getClientFd(), ERR_CHANOPRIVSNEEDED(msg.params[0]).c_str(), ERR_CHANOPRIVSNEEDED(msg.params[0]).length(), 0);
        return(-1);
    }
    std::string topic_message;
    topic_message = TOPIC_MESSAGE(USER(client->getNickName(),client->getUserName(),client->getIPaddress()), channelIt->second->getChannelName());
    if(msg.trailing_flag == 1)
    {
        topic_message.append(msg.trailing + " \r\n");
        channelIt->second->setTopic(msg.trailing);
    }
    sendTopicMsg(topic_message,channelIt->second, client, msg.trailing_flag);
    return(0);
}