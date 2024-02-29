#include "../../includes/Server.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Reply.hpp"
#include <time.h>

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
	std::string readline = readFile("./motd.txt");
	std::istringstream iss(readline);
	std::string motd_line;

	Client->setSendbuf(RPL_WELCOME(hostname, Client->getServerName(), nick, userIP));
	Client->addSendbuf(RPL_YOURHOST(hostname, nick, Client->getServerName()));
	Client->addSendbuf(RPL_CREATED(hostname, nick, currentDateTime()));
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
