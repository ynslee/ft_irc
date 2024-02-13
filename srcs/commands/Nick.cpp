#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"

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

int is_valid_nick(std::string new_nick)
{
    if(!isdigit(new_nick[0]) || !isalpha(new_nick[0]))
        return (-1);
    
    std::string::iterator it;

    for(it = new_nick.begin(); it != new_nick.end(); it++)
    {
        if(!std::isalnum(*it) && *it != '[' && *it != ']' && *it != '{' && *it != '}' && *it != '\\' && *it != '|')
            return(-1);
        else if(!std::isspace(*it))
            return(-1);
    }
    return(0);
}

int Server::cmd_nick(Message &msg, int client_fd)
{
    std::string message;
    std::string servername("hola");

    Server server;
    if(msg.params.size() < 1)
    {
        message = ERR_NONICKNAMEGIVEN(servername);
        send(client_fd, message.c_str(), message.length(), 0);
        return(-1);
    }
    std::string new_nick = msg.params.front();
    if(is_valid_nick(new_nick))
    {
        message = ERR_ERRONEUSNICKNAME(servername, new_nick);
        send(client_fd, message.c_str(), message.length(), 0);
        return(-1);
    }
    return(0);
}