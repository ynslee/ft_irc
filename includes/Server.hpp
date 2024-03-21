
#ifndef SERVER_HPP
# define SERVER_HPP

# define MAXCLIENTS 32
# include "Common.hpp"
# include "Client.hpp"
# include "Message.hpp"
# include "Commands.hpp"
# include "Channel.hpp"

extern bool serverShutdown;

class Client;
class Message;
class Channel;

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
		QUIT,
		PING,
		INVALID,
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
		std::map<std::string, Channel*> _channels;

	public:
		Server(std::string port, std::string password);
		~Server();
		int getClientId();
		void setClientId(const int id);
		int serverSetup();
		int acceptPendingConnections();
		int recieveMsg(int new_fd, int i, std::ofstream &log);
		int sendMsg(int send_fd, std::ofstream &log);
		int pollLoop();
		void closeClient(int i, int fd, Client *client);
		int setMessage(std::string msg, std::ofstream &log);
		int findCommand(int client_fd);
		int getCommandType(std::string command);
		const std::string &getServerName() const;
		std::vector<std::string> &getNicknames();
		void removeClientfromPollAndMap(int fd);
		std::map<std::string, Channel*> &getChannels();
		std::map<int, Client*> &getClients();
		bool ChannelExists(std::map<std::string, Channel*> &channels, std::string &channelName);
};

std::string extractInput(std::map<int, Client *> _clients, int client_fd);

#endif
