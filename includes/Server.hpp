
#ifndef SERVER_HPP
# define SERVER_HPP

# define MAXCLIENTS 32
# include "Common.hpp"
# include "Client.hpp"
# include "Message.hpp"

class Client;

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
		int _pollfd_count;
		int _client_id;
		std::map<int, Client*>	_clients;

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
		void setMessage(const char* msg);
		int findCommand(int client_fd);
		int getCommandType(std::string command);
		const std::string &getServerName() const;
};

#endif
