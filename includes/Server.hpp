
#ifndef SERVER_HPP
# define SERVER_HPP

# define MAXCLIENTS 32
# include "Common.hpp"

class Server
{
	private:
		Server();
		std::string port;
		std::string password;
		std::vector<struct pollfd> pfds;
		int pollfd_count;
		std::string message;
		int client_id;

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
		void shut_down_server(int i, int fd);
};

#endif
