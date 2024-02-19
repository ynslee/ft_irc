#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief 
 * The OPER command is used by a normal user to obtain IRC operator privileges.
 * SYNTAX : OPER <name> <password>
 *
 * ERR_NEEDMOREPARAMS (461)
 * ERR_PASSWDMISMATCH (462)
 * ERR_NOOPERHOST (491)
 * RPL_YOUREOPER (381)
 * <username> needs to be at least 1 character long.
 */

int cmdOper(Message &msg, Client *Client, std::string password)
{
    std::string hostname = Client->getHostName();

    if(Client->getRegisteration() == 0)
    {
        send(Client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
        return(-1);
    }
    else if(msg.params.size() < 2)
    {
        send(Client->getClientFd(), ERR_NEEDMOREPARAMS(hostname).c_str(), ERR_NEEDMOREPARAMS(hostname).length(), 0);
        return(-1);
    }
    else if(msg.params[1] != password)
    {
        send(Client->getClientFd(), ERR_PASSWDMISMATCH(hostname).c_str(), ERR_PASSWDMISMATCH(hostname).length(), 0);
        return(-1);
    }
    else if(msg.params[0] != Client->getUserName())
    {
        send(Client->getClientFd(), ERR_NOOPERHOST(hostname).c_str(), ERR_NOOPERHOST(hostname).length(), 0);
        return(-1);
    }
    else
    {
        Client->setIsOperator(true);
        send(Client->getClientFd(), RPL_YOUREOPER(hostname,Client->getUserName()).c_str(), RPL_YOUREOPER(hostname,Client->getUserName()).length(), 0);
    }
    return(0);
}