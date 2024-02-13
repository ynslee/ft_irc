
#ifndef SERVER_HPP
# define SERVER_HPP

# define MAXCLIENTS 32
# include "Common.hpp"
# include "Client.hpp"

class Client;

class Server
{
	enum command {
		PASS,
		NICK,
		USER,
		JOIN,
		INVITE,
		KICK,
		TOPIC,
		MODE
		// what else?
	};

	private:
		Server();
		std::string port;
		std::string password;
		std::vector<struct pollfd> pfds;
		int pollfd_count;
		int client_id;
		std::map<int, Client*>	_clients;

	public:
		Server(std::string port, std::string password);
		~Server();
		int getClientId();
		void setClientId(const int id);
		void setMessage(const char* msg);
		int serverSetup(std::string prt);
		int acceptPendingConnections();
		int recieve_msg(int new_fd, int i);
		int send_msg(int new_fd);
		int poll_loop();
		void close_client(int i, int fd);
};

#endif
