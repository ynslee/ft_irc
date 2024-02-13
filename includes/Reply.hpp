
#ifndef REPLY_HPP
# define REPLY_HPP

//ERROR REPLIES
# define ERR_NEEDMOREPARAMS(servename)(":" + servername + " 461 * :Not enough parameters" + "\r\n")	
# define ERR_ALREADYREGISTRED(servername)(":" + servername + " 462 * :You may not reregister" + "\r\n")
# define ERR_PASSWDMISMATCH(servername)(":" + servername + " 464 * :Password incorrect" + "\r\n")
# define ERR_NONICKNAMEGIVEN(servermame)(":" + servername + " 431 * :No nickname given" + "\r\n")
# define ERR_ERRONEUSNICKNAME(servername, nick)(":" + servername + " 432 * " + nick + " :Erroneus nickname" + "\r\n")
# define ERR_NICKNAMEINUSE(servername, nick)(":" + servername + " 433 * " + nick +  " :Nickname is already in use" + "\r\n")
# define ERR_NICKCOLLISION(servername, nick, user, host)(":" + servername + " 436 * " + nick + " Nickname collision KILL from " + user + "@" + host + + "\r\n")
//NORMAL REPLIES
# define NICK_REPLY(old_nick, usesrname, host, new_nick)(":" + old_nick + "!~" + usesrname + "@" + host + " Nick :" + new_nick+ "\r\n")

#endif