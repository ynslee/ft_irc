
#include "../../includes/Server.hpp"

/**
 * @brief PASS command for setting 'connection password'
 * Syntax: PASS <password>
 * 
 * 	ERR_NEEDMOREPARAMS (461)
 * 	ERR_ALREADYREGISTERED (462)
 * 	ERR_PASSWDMISMATCH (464)
 * 
 * Example : /PASS secretpassword
 */
void pass_command(Client *client, Message *msg)
{
	//if (password is empty){
		//send ERR_NEEDMOREPARAMS
	//}
	else if(client->() == 3)
	

}
