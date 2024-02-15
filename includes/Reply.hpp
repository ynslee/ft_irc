
#ifndef REPLY_HPP
# define REPLY_HPP

//ERROR REPLIES
# define ERR_NEEDMOREPARAMS(hostname)(":" + hostname+ " 461 * :Not enough parameters" + "\r\n")	
# define ERR_ALREADYREGISTRED(hostname)(":" + hostname + " 462 * :You may not reregister" + "\r\n")
# define ERR_PASSWDMISMATCH(hostname)(":" + hostname + " 464 * :Password incorrect" + "\r\n")
# define ERR_NOTREGISTERED(hostname)(":" + hostname + " 451 * :You have not registered" + "\r\n")
# define ERR_NOSUCHSERVER(hostname, servername)(":" + hostname + " 402 " + servername + " :No such server" + "\r\n")


//NORMAL REPLIES
# define NICK_REPLY(old_nick, usesrname, userIP, new_nick)(":" + old_nick + "!~" + usesrname + "@" + userIP + " Nick :" + new_nick+ "\r\n")
# define RPL_MOTDSTART(hostname, usersname)(":" + hostname+ " 375 * :- " + username + " Message of the day - " + "\r\n")
# define RPL_MOTD(hostname, client, motd_line) (":" + hostname + " 372 " + client + " :" + motd_line + "\r\n")
# define RPL_ENDOFMOTD(hostname, client) (":" + hostname + " 376 " + client + " :End of /MOTD command.\r\n")

#endif
