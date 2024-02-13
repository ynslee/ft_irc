
#ifndef REPLY_HPP
# define REPLY_HPP

//ERROR REPLIES
# define ERR_NEEDMOREPARAMS(servename)(":" + servername + " 461 * :Not enough parameters" + "\r\n")	
# define ERR_ALREADYREGISTRED(servername)(":" + servername + " 462 * :You may not reregister" + "\r\n")
# define ERR_PASSWDMISMATCH(servername)(":" + servername + " 464 * :Password incorrect" + "\r\n")


//NORMAL REPLIES

#endif