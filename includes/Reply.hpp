
#ifndef REPLY_HPP
# define REPLY_HPP

//ERROR REPLIES
# define ERR_NEEDMOREPARAMS(serverName)(":" + servername + " 461 * :Not enough parameters" + "\r\n")	
# define ERR_ALREADYREGISTRED(servername)(":" + servername + " 462 * :You may not reregister" + "\r\n")
# define ERR_PASSWDMISMATCH(servername)(":" + servername + " 464 * :Password incorrect" + "\r\n")

# define NICK_REPLY(old_nick, usesrname, host, new_nick)(":" + old_nick + "!~" + usesrname + "@" + host + " Nick :" + new_nick+ "\r\n")
//NORMAL REPLIES
# define NICK_REPLY(old_nick, usesrname, userIP, new_nick)(":" + old_nick + "!~" + usesrname + "@" + userIP + " Nick :" + new_nick+ "\r\n")

#endif
