#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief The QUIT command is used to terminate a client’s connection to the server. The server acknowledges
 * this by replying with an ERROR message and closing the connection to the client.
 * Syntax: PASS [<reason>]
 *
 * This message may also be sent from the server to a client to show that a client has exited from the network.
 * This is typically only dispatched to clients that share a channel with the exiting user.
 * When the QUIT message is sent to clients, <source> represents the client that has exited the network.
 *
 *
 * Example : /QUIT Gone to have lunc
 */

/*---------------------------------------IMPORTANT------------------------------------------------------------*/
/*The quit message stuff is part of the channel stuff, soo in a channel we have to send a message to all
the clients in the channel, if one of the users quit. But if you are not in a channel just quit the server. That
part will be done after we set the channel*/

void sendMsgtoClients(std::string message, Client *client, Channel *channel)
{
    if(client == nullptr)
        return ;
    std::map<std::string, Client *> clients_list = channel->getClientList();
    std::map<std::string, Client *>::const_iterator it;
    for(it = clients_list.begin(); it != clients_list.end() ; ++it)
    {
        if(client != NULL && it->first == client->getNickName())
            continue;
        it->second->setSendbuf(message);       
    }
    channel->removeFromChannel(client->getNickName());
}


void cmdQuit(Message &msg, Client *Client, std::map<std::string, Channel*> &channels,  std::vector<std::string> &nick_names)
{

    std::string quit_message;
    quit_message = QUIT_MESSAGE(Client->getNickName(), Client->getUserName(), Client->getIPaddress());
    if(msg.params.size())
        quit_message.append(msg.params.front() + "\r\n");
    else
        quit_message.append("\r\n");
    std::vector<std::string>::iterator it;
    for(it = Client->getChannelsJoined().begin(); it < Client->getChannelsJoined().end(); it++)
    {
        std::string channel_name = *it;
        std::map<std::string, Channel*>::iterator channelIt = channels.find(channel_name);  
        if(channelIt != channels.end())
        {
            sendMsgtoClients(quit_message,Client,channelIt->second);
            if(channelIt->second->getClientList().size() == 0)
            {
                std::cout << "Channel " << channelIt->second->getChannelName() << " deleted ";
                delete (channelIt->second);
                channels.erase(channelIt);
                continue ;                
            }
        }
    }
    std::vector<std::string>::iterator iter = std::find(nick_names.begin(),nick_names.end(), Client->getNickName());
    nick_names.erase(iter);
    close(Client->getClientFd());
    return ;
}
