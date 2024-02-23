
#ifndef REPLY_HPP
# define REPLY_HPP

# define USER(nick, username, userIP)(":" + nick + "!" + username + "@" + userIP)

//ERROR REPLIES
# define ERR_NEEDMOREPARAMS(hostname)(":" + hostname + " 461 * :Not enough parameters" + "\r\n")	
# define ERR_ALREADYREGISTRED(hostname)(":" + hostname + " 462 * :You may not reregister" + "\r\n")
# define ERR_PASSWDMISMATCH(hostname)(":" + hostname + " 464 * :Password incorrect" + "\r\n")
# define ERR_NONICKNAMEGIVEN(hostnmae)(":" + hostname + " 431 * :No nickname given" + "\r\n")
# define ERR_ERRONEUSNICKNAME(hostname, nick)(":" + hostname + " 432 * " + nick + " :Erroneus nickname" + "\r\n")
# define ERR_NICKNAMEINUSE(hostname, nick)(":" + hostname + " 433 * " + nick +  " :Nickname is already in use" + "\r\n")
# define ERR_NICKCOLLISION(hostname, nick, user, host)(":" + hostname + " 436 * " + nick + " Nickname collision KILL from " + user + "@" + host + + "\r\n")
# define ERR_NOTREGISTERED(hostname)(":" + hostname + " 451 * :You have not registered" + "\r\n")
# define ERR_NOSUCHSERVER(hostname, servername)(":" + hostname + " 402 " + servername + " :No such server" + "\r\n")
# define ERR_NOOPERHOST(hostname)(":" + hostname + " 491 * :You have not registered" + "\r\n")
# define ERR_TOOMANYCHANNELS(username, channel) ("405 " + username + " " + channel + " :You have joined too many channels\r\n")
# define ERR_BADCHANNELKEY(username, channel) ("475 " + username + " " + channel + " :Cannot join channel (+k)\r\n")
# define ERR_CHANNELISFULL(username, channel) ("471 " + username + " " + channel + " :Cannot join channel (+l)\r\n")
# define ERR_INVITEONLYCHAN(username, channel) ("473 " + username + " " + channel + " :Cannot join channel (+i)\r\n")

//NORMAL REPLIES
# define QUIT_MESSAGE(nick, username, userIP)(":" + nick + "!" + username + "@" + userIP + " QUIT :Quit: ")
# define NICK_REPLY(old_nick, usesrname, userIP, new_nick)(":" + old_nick + "!~" + usesrname + "@" + userIP + " Nick :" + new_nick+ "\r\n")
# define RPL_MOTDSTART(hostname, usersname)(":" + hostname+ " 375 * :- " + username + " Message of the day - " + "\r\n")
# define RPL_MOTD(hostname, username, motd_line) (":" + hostname + " 372 " + username + " :" + motd_line + "\r\n")
# define RPL_ENDOFMOTD(hostname, username) (":" + hostname + " 376 " + username + " :End of /MOTD command.\r\n")
# define RPL_WELCOME(hostname, username, servername, nick, userIP)(":" + hostname + " 001 " + username + " :Welcome to the Internet Relay Network " + servername + "!" + nick + "@" + userIP + "\r\n")
# define RPL_YOURHOST(hostname, username, servername)(":" + hostname + " 002 " + username + " :Your host is " + servername + ", running version 1.0\r\n")
# define RPL_CREATED(hostname, username, date)(":" + hostname + " 003 " + username + " :This server was created " + date + "\r\n")
# define RPL_MODE(username, mode)(":" + username + " MODE " + username + " :" + mode + "\r\n")
# define RPL_YOUREOPER(hostname, username)(":" + hostname + " 381 " + username + " :You are now an IRC operator " + "\r\n")
# define RPL_JOIN(USER, channel, realname)(USER + " JOIN " + channel + " * :" + realname + "\r\n")


#endif
