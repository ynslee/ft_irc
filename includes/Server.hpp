
#ifndef SERVER_HPP
# define SERVER_HPP

# define MAXCLIENTS 32
# include "Common.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "Commands.hpp"

class Client;
class Message;

	enum command {
		CAP,
		PASS,
		NICK,
		USER,
		JOIN,
		INVITE,
		KICK,
		TOPIC,
		MODE,
		MOTD,
		PRIVMSG,
		WHOIS,
		KILL,
		OPER,
		INVALID,
		// what else?
	};

class Server
{
	private:
		Server();
		std::string _port;
		std::string _serverName;
		std::string _password;
		std::vector<struct pollfd> _pfds;
		int _pollfdCount;
		int _clientId;
		std::map<int, Client*>	_clients;
		std::vector<std::string> _nicknames;

	public:
		Server(std::string port, std::string password);
		~Server();
		int getClientId();
		void setClientId(const int id);
		int serverSetup();
		int acceptPendingConnections();
		int recieveMsg(int new_fd, int i);
		int sendMsg(int send_fd);
		int pollLoop();
		void closeClient(int i, int fd);
		void setMessage(std::string msg);
		int findCommand(int client_fd);
		int getCommandType(std::string command);
		const std::string &getServerName() const;
		static bool findNick(std::string nick);
		std::vector<std::string> &getNicknames();
};

std::string extractInput(std::map<int, Client *> _clients, int client_fd);

#endif
