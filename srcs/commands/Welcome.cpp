#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include <time.h>

/**
 * @brief The server sends Replies 001 to 004 to a user upon successful registration.

001    RPL_WELCOME "Welcome to the Internet Relay Network, <nick>!<user>@<host>"
002    RPL_YOURHOST "Your host is <servername>, running version <ver>"
003    RPL_CREATED "This server was created <date>"
004    RPL_MYINFO "<servername> <version> <available user modes>, <available channel modes>"

*/
static std::string readFile(const std::string &filename)
{
	std::ifstream file(filename);
	std::stringstream buf;

	if (file)
		buf << file.rdbuf();
	return buf.str();
}

static const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void welcomeUser(Client *Client)
{
	std::string hostname = Client->getHostName();
	std::string username = Client->getUserName();
	std::string userIP = Client->getIPaddress();
	std::string nick = Client->getNickName();
	std::string mode = Client->getMode();
	std::string userModes = "+i";
	std::string channelModes = "+itkol";
	std::string version = "v1.0";
	std::string readline = readFile("./motd.txt");
	std::istringstream iss(readline);
	std::string motd_line;

	Client->setSendbuf(RPL_WELCOME(hostname, Client->getServerName(), nick, userIP));
	Client->addSendbuf(RPL_YOURHOST(hostname, nick, Client->getServerName()));
	Client->addSendbuf(RPL_CREATED(hostname, nick, currentDateTime()));
	Client->addSendbuf(RPL_MYINFO(hostname, version, userModes, channelModes));
	Client->addSendbuf(RPL_MOTDSTART(hostname, nick));
	while(std::getline(iss, motd_line))
	{
		if (motd_line.empty() == false)
			Client->addSendbuf(RPL_MOTD(hostname, nick, motd_line));
	}
	if (readline.empty() == false)
		Client->addSendbuf(RPL_ENDOFMOTD(hostname, nick));
	Client->addSendbuf(RPL_MODE(nick, mode));
	Client->setWelcomeSent(1);
}
