
#ifndef REPLY_HPP
# define REPLY_HPP

//ERROR REPLIES
# define ERR_NEEDMOREPARAMS(_serverName)(":" + _serverName + " 461 * :Not enough parameters" + "\r\n")
# define ERR_ALREADYREGISTRED(_serverName)(":" + _serverName + " 462 * :You may not reregister" + "\r\n")
# define ERR_PASSWDMISMATCH(_serverName)(":" + _serverName + " 464 * :Password incorrect" + "\r\n")

# define NICK_REPLY(old_nick, usesrname, host, new_nick)(":" + old_nick + "!~" + usesrname + "@" + host + " Nick :" + new_nick+ "\r\n")
//NORMAL REPLIES

#endif
