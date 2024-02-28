
#ifndef REPLY_HPP
# define REPLY_HPP

# define USER(nick, username, userIP)(":" + nick + "!~" + username + "@" + userIP)

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
# define ERR_NOSUCHCHANNEL(username, channel) ("403 " + username + " " + channel + " :No such channel\r\n")
# define ERR_UNKNOWNMODE()("472 * :Unknown MODE flag\r\n")
# define ERR_NOSUCHNICK(nick)("401 * " + nick + " :No such nick/channel" + "\r\n")
// # define ERR_NOSUCHCHANNEL(channel)("403 * " + channel + " :No such channel" + "\r\n")
# define ERR_CANNOTSENDTOCHAN(hostname, channel)(":" + hostname + " 404 * " + channel + " :Cannot send to channel" + "\r\n")
# define ERR_NORECIPIENT(hostname)(":" + hostname + " 411 * :No recipient given" + "\r\n")
# define ERR_NOTEXTTOSEND(hostname)(":" + hostname + " 412 * :No text to send" + "\r\n")
# define ERR_TOOMANYTARGETS(hostname)(":" + hostname + " 407 * :Too many recipients" + "\r\n")
# define ERR_CHANOPRIVSNEEDED(username, channel) ("482 " + username + " " + channel + " :You're not channel operator\r\n")
# define ERR_NOTONCHANNEL(username,channel) ("442 " + username + " " + channel + " :You're not on that channel\r\n")
# define ERR_USERNOTINCHANNEL(username,nick,channel) ("441 " + username + " " + nick + " " + channel + " :Not on that channel\r\n")

//NORMAL REPLIES
# define QUIT_MESSAGE(nick, username, userIP)(":" + nick + "!" + username + "@" + userIP + " QUIT :Quit: ")
# define KICK_MESSAGE(nick, channel)(":Command to kick " + nick + " from " + channel)
# define NICK_REPLY(old_nick, usesrname, userIP, new_nick)(":" + old_nick + "!~" + usesrname + "@" + userIP + " Nick :" + new_nick+ "\r\n")
# define RPL_MOTDSTART(hostname, username)(":" + hostname+ " 375 * :- " + username + " Message of the day - " + "\r\n")
# define RPL_MOTD(hostname, username, motd_line) (":" + hostname + " 372 " + username + " :" + motd_line + "\r\n")
# define RPL_ENDOFMOTD(hostname, username) (":" + hostname + " 376 " + username + " :End of /MOTD command.\r\n")
# define RPL_WELCOME(hostname, username, servername, nick, userIP)(":" + hostname + " 001 " + username + " :Welcome to the Internet Relay Network " + servername + "!" + nick + "@" + userIP + "\r\n")
# define RPL_YOURHOST(hostname, username, servername)(":" + hostname + " 002 " + username + " :Your host is " + servername + ", running version 1.0\r\n")
# define RPL_CREATED(hostname, username, date)(":" + hostname + " 003 " + username + " :This server was created " + date + "\r\n")
# define RPL_MODE(username, mode)(":" + username + " MODE " + username + " :" + mode + "\r\n")
# define RPL_YOUREOPER(hostname, username)(":" + hostname + " 381 " + username + " :You are now an IRC operator " + "\r\n")
# define RPL_JOIN(USER, channel, realname)(USER + " JOIN " + channel + " * :" + realname + "\r\n")
# define RPL_NAMREPLY(hostname, username, channel, nicklist) (":" + hostname + " 353 " + username + " = " + channel + " :" + nicklist + "\r\n")
# define RPL_ENDOFNAMES(hostname, username, channel) (":" + hostname + " 366 " + username + " " + channel + " :End of /NAMES list.\r\n")
# define RPL_TOPIC(hostname, username, channel, topic) (":" + hostname + " 332 " + username + " " + channel + " :" + topic + "\r\n")
# define RPL_NOTOPIC(hostname, username, channel) (":" + hostname + " 331 " + username + " " + channel + " :No topic is set\r\n")
# define RPL_PRIVMSG(USER, target, text) (USER + " PRIVMSG " + target + " :" + text + "\r\n")

#endif
