#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"

/**
 * @brief If <target> is not given, the MOTD of the server the client is 
 * connected to should be returned.
 * 
 *  Syntax : MOTD [<target>]
 * 
 *  If the MOTD can be found, one RPL_MOTDSTART numeric is returned, followed by
 *  one or more RPL_MOTD numeric, then one RPL_ENDOFMOTD numeric.
 * 
 * 	If the MOTD does not exist or could not be found, the ERR_NOMOTD numeric is returned.
 * 
 *  Numeric replies:
 *   ERR_NOSUCHSERVER (402)
 *   ERR_NOMOTD (422)
 *   RPL_MOTDSTART (375)
 *   RPL_MOTD (372)
 *   RPL_ENDOFMOTD (376)
 * 
 */

static std::string readFile(const std::string &filename)
{
	std::ifstream file(filename);
	std::stringstream buf;

	if (file)
		buf << file.rdbuf();
	return buf.str();
}

int cmdMotd(Message &msg, Client *Client)
{
	std::string hostname = Client->getHostName();
	std::string nickname = Client->getNickName();
	std::string readline = readFile("./motd.txt");
	std::istringstream iss(readline);
	std::string motd_line;

	if (Client->getWelcomeSent() != 1)
	{
		send(Client->getClientFd(), ERR_NOTREGISTERED(hostname).c_str(), ERR_NOTREGISTERED(hostname).length(), 0);
		return (-1);
	}
	if (msg.params.empty() == true)
	{
		Client->setSendbuf(RPL_MOTDSTART(hostname, nickname));
		while(std::getline(iss, motd_line))
		{
			if (motd_line.empty() == false)
				Client->addSendbuf(RPL_MOTD(hostname, nickname, motd_line));
		}
		if (readline.empty() == false)
			Client->addSendbuf(RPL_ENDOFMOTD(hostname, nickname));
	}
	else if (msg.params[0].compare("localhost") != 0 || msg.params[0].compare("127.0.0.1") != 0)
	{
		send(Client->getClientFd(), ERR_NOSUCHSERVER(hostname, msg.params[0]).c_str(), ERR_NOSUCHSERVER(hostname, msg.params[0]).length(), 0);
		return (-1);
	}
	return (0);
}