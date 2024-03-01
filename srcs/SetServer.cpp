#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Message.hpp"
#include "../includes/Commands.hpp"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Server::serverSetup()
{
	int status, socketfd;
	struct addrinfo hints, *p;
	struct addrinfo *serverinfo;
	int yes = 1;

	this->_serverName = "🐾TYCHUNEN SERVER🐾";
	const char *port = _port.c_str();
	std::cout << "port " << this->_port << std::endl;

// SETTING UP getaddrinfo()
	std::memset(&hints, 0, sizeof(hints));	// to empty the struct
	hints.ai_family = AF_UNSPEC;		// IPv4 or IPv6 agnostic
	hints.ai_socktype = SOCK_STREAM;	// TCP sockets
	hints.ai_flags = AI_PASSIVE;		// to fill in IP
	if ((status = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0)
	{
		std::cerr << "Error in getaddrinfo: " << gai_strerror(status) << std::endl;
		return (-1);
	}

// loop trough possible socket addresess and connect to first possible one
// create socket
	for (p = serverinfo; p != NULL; p = p->ai_next)
	{
		if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue ;
		// std::cout << socketfd << std::endl;
// to manage error “Address already in use.”
		if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			std::cerr << "Could not set socket to be reusable: error in setsockopt()" << strerror(errno) << std::endl;
			return (-1);
		}
		fcntl(socketfd, F_SETFL, O_NONBLOCK);
		if (bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1)
		{
			close(socketfd);
			continue ;
		}
		break ;
	}
	freeaddrinfo(serverinfo);
	if (p == NULL)
	{
		std::cerr << "Server failed to bind()" << std::endl;
		return (-1);
	}

	if (listen(socketfd, MAXCLIENTS) == -1)
	{
		std::cerr << "Error in listen()" << std::endl;
		return (-1);
	}
	struct pollfd poll_fd;
	poll_fd.fd = socketfd;
	poll_fd.events = POLLIN;
	this->_pfds.push_back(poll_fd);
	std::cout << "listener socket is :" <<this->_pfds[0].fd << std::endl;
	return (0);
}

int Server::pollLoop()
{
	int poll_count;
	this->_pollfdCount = this->_pfds.size();
	// std::cout << this->_pollfdCount << std::endl;

	while(42)
	{
		poll_count = poll(&this->_pfds[0], this->_pollfdCount, 0);
		if (poll_count == -1)
		{
			std::cerr << "Poll Error" << std::endl;
			return (-1);
		}
		for(int i = 0; i < this->_pollfdCount; i++)
		{
			if(this->_pfds[i].revents & POLLIN)
			{
				if(this->_pfds[i].fd == this->_pfds[0].fd)
				{
					if(acceptPendingConnections() == -1)
						return(-1);
				}
				else
				{
					if(recieveMsg(this->_pfds[i].fd, i) == -1)
						continue;
				}
			}
			else if (this->_pfds[i].revents & POLLOUT)
			{
				for (int i = 0; i < this->_pollfdCount; i++)
				{
					if (this->_pfds[i].fd == getClientId())
					{
						if (sendMsg(this->_pfds[i].fd) == -1)
							continue;
					}
				}
			}
			else if (this->_pfds[i].revents & POLLERR)
			{
				return (-1);
			}
		}
	}
	for(int i = 0; i < this->_pollfdCount; i++)
		close(this->_pfds[i].fd);
	return(0);
}

int Server::acceptPendingConnections()
{
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	int new_fd;
	char s[INET6_ADDRSTRLEN];
	struct pollfd poll_fd;
	char hostname[258];
	// Client new_client(new_fd);

	addr_len = sizeof(their_addr);
	new_fd = accept(this->_pfds[0].fd, (struct sockaddr *)&their_addr, &addr_len);
	if (new_fd == -1)
	{
		std::perror("Could not create a newfd in accept()");
		return(-1);
	}
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	if(this->_pollfdCount < MAXCLIENTS + 1)
	{
		poll_fd.fd = new_fd;
		poll_fd.events = POLLIN | POLLOUT;
		this->_pfds.push_back(poll_fd);
	}
	else
	{
		send(new_fd, "[IRCSERV] You cannot join, the server is already full", 53, 0);
		close(new_fd);
	}
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
	std::cout << "New conection from " << s << " on socket :" << new_fd << std::endl;
	_clients.insert(std::make_pair(new_fd, new Client(new_fd)));
	_clients[new_fd]->setIPaddress("::ffff:127.0.0.1");
	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		std::cerr << "Error in gethostname()" << std::endl;
		return (-1);
	}
	_clients[new_fd]->setHostName(hostname);
	std::cout << "host name is" << hostname << std::endl;
	this->_pollfdCount = this->_pfds.size();
	return (0);
}

int Server::recieveMsg(int client_fd, int i)
{
	char buf[512];
	int readcount;

	memset(buf, 0, sizeof(buf));
	readcount = recv(client_fd, buf, sizeof(buf), 0);
	if (readcount < 0)
	{
		if (errno != EWOULDBLOCK) // no data to read
		{
			std::cerr << "Error in recv()" << std::endl;
			closeClient(i, client_fd);
			return (-1);
		}
	}
	else if (readcount == 0)
	{
		std::cerr << "Peer has closed connection" << std::endl;
		closeClient(i, client_fd);
		return (0);
	}
	else
	{
		setClientId(client_fd);
		if (setMessage(static_cast<std::string>(buf)) == -1)
			return (-1);
		if(findCommand(client_fd) == -1)
			return(-1);
		// std::cout << "received<< " << buf << std::endl;
		return (0);
	}
	return (-1);
}

int Server::findCommand(int client_fd)
{
	while (1)
	{

		if (_clients[client_fd]->getReadbuf().empty())
			return (0);
		std::string input = extractInput(_clients, client_fd);
		Message msg(input);

		std::cout << "read buf leftover is " << _clients[client_fd]->getReadbuf() << std::endl;

		int i = getCommandType(msg.command);
		switch(i)
		{
			case command::CAP:
			{
				cmdCap(msg, _clients[client_fd]);
				break ;
			}
			case command::PASS:
				if(cmdPass(msg, _clients[client_fd], this->_password) == -1)
					return(-1);
				break ;
			case command::NICK:
			{
				if(cmdNick(msg,_clients[client_fd], getNicknames()))
					return(-1);
				break ;
			}
			case command::USER:
				if(cmdUser(msg, _clients[client_fd]) == -1)
					return(-1);
				break ;
			case command::JOIN:
			{
				if(cmdJoin(msg, _clients[client_fd], _channels) == -1)
					return(-1);
				break;
			}
			case command::MODE:
				if (cmdMode(msg, _clients[client_fd], _channels) == -1)
					return(-1);
				break ;
			case command::MOTD:
				if (cmdMotd(msg, _clients[client_fd]) == -1)
					return(-1);
				break ;
			case command::OPER:
				if (cmdOper(msg, _clients[client_fd]) == -1)
					return(-1);
				break ;
			case command::PRIVMSG:
				if (cmdPrivmsg(msg, _clients[client_fd], _channels, _clients) == -1)
					return(-1);
				break ;
			case command::QUIT:
			{
				cmdQuit(msg, _clients[client_fd],_channels, getNicknames());
				removeClientfromPoll(client_fd);
				return(0);
			}
			case command::INVALID:
			{
				std::cout << "Invalid command" << std::endl;
				return (-1);
			}
		}
	}
	return (0);
}

std::string extractInput(std::map<int, Client *> _clients, int client_fd)
{
	size_t pos = 0;

	pos = _clients[client_fd]->getReadbuf().find('\n');
	std::string input =  _clients[client_fd]->getReadbuf().substr(0, pos);
	std::string temp = _clients[client_fd]->getReadbuf();
	temp.erase(0, pos + 1);
	_clients[client_fd]->setReadbuf(temp);
	return (input);
}

int Server::sendMsg(int client_fd)
{
	std::string message;

	message.clear();
	std::map<int, Client*>::iterator it;
	for(it=_clients.begin(); it!=_clients.end(); it++)
	{
		int key = it->first;
		if(key == client_fd)
		{
			message = it->second->getSendbuf();
			break;
		}
	}
	if (message.empty())
	{
		return (0);
	}
	int len = message.length();
	int send_readcount = send(client_fd, message.c_str(), len, 0);
	if (send_readcount == -1)
	{
		// std::cerr << "Error in send()" << std::endl;
		return (-1);
	}
	_clients[client_fd]->setSendbuf("");
	std::cout<< "sending>>> " << message << std::endl;
	return (0);
}

std::map<std::string, Channel*> &Server::getChannels()
{
	return(this->_channels);
}

std::map<int, Client*> &Server::getClients()
{
	return(this->_clients);
}


Server::Server(std::string port, std::string password): _port(port), _password(password)
{
	/// check some error stuff;
	int port_number;

	port_number = std::stoi(_port);
	try
	{
		if(port_number < 1024 || port_number > 65535)
			throw std::runtime_error("Invalid port number");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return ;
	}
	try
	{
		if(_password.empty() == true)
			throw std::exception();
	}
	catch(const std::exception& e)
	{
		std::cerr << "password is empty" << std::endl;
		return ;
	}
	try
	{
		_channels.clear();
		if (serverSetup() < 0)
			throw std::runtime_error("Could not set server up");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return ;
	}
}


Server::~Server()
{
	std::map<int, Client*>::iterator it;
	for(it=_clients.begin(); it!=_clients.end(); it++)
	{
		delete it->second;
	}
	std::map<std::string, Channel*>::iterator it2;
	for(it2=_channels.begin(); it2!=_channels.end(); it++)
	{
		delete it->second;
	}
}

int Server::getClientId()
{
	return(this->_clientId);
}

void Server::setClientId(const int id)
{
	this->_clientId = id;
}

std::vector<std::string> &Server::getNicknames()
{
	return(this->_nicknames);
}

int Server::setMessage(std::string msg)
{
	std::map<int, Client*>::iterator it;
	for(it=_clients.begin(); it!=_clients.end(); it++)
	{
		int key = it->first;
		if(key == this->_clientId)
		{
			it->second->addReadbuf(msg);
			msg.clear();
			if (it->second->getReadbuf().find('\n') == std::string::npos)
				return (-1);
		}
	}
	return (0);
}

const std::string &Server::getServerName() const
{
	return (this->_serverName);
}
