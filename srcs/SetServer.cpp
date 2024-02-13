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

int Server::serverSetup(std::string prt)
{
	int status, socketfd;
	struct addrinfo hints, *p;
	struct addrinfo *serverinfo;
	int yes = 1;

	const char *port = prt.c_str();
	std::cout << "port " << port << std::endl;

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
	this->pfds.push_back(poll_fd);
	std::cout << "listner socket is :" <<this->pfds[0].fd << std::endl;
	return (0);
}

int Server::poll_loop()
{
	int poll_count;
	this->pollfd_count = this->pfds.size();
	// std::cout << this->pollfd_count << std::endl;

	while(42)
	{
		poll_count = poll(&this->pfds[0], this->pollfd_count, 0);
		if (poll_count == -1)
		{
			std::cerr << "Poll Error" << std::endl;
			return (-1);
		}
		for(int i = 0; i < this->pollfd_count; i++)
		{
			if(this->pfds[i].revents & POLLIN)
			{
				if(this->pfds[i].fd == this->pfds[0].fd)
				{
					if(acceptPendingConnections())
						return(-1);
				}
				else
				{
					if(recieve_msg(this->pfds[i].fd, i) == -1)
							return(-1);
				}
			}
			else if (this->pfds[i].revents & POLLOUT){
				for (int i = 0; i < this->pollfd_count; i++)
				{
					if (this->pfds[i].fd == getClientId())
					{
						if (send_msg(this->pfds[i].fd) == -1)
						{
							// std::perror("Error in send()");
							break;
						}
					}
				}
			}
			else if (this->pfds[i].revents & POLLERR)
			{
				return (-1);
			}
		}
	}
	for(int i = 0; i < this->pollfd_count; i++)
		close(this->pfds[i].fd);
	return(0);
}

int Server::acceptPendingConnections()
{
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	int new_fd;
	char s[INET6_ADDRSTRLEN];
	struct pollfd poll_fd;
	// Client new_client(new_fd);

	addr_len = sizeof(their_addr);
	new_fd = accept(this->pfds[0].fd, (struct sockaddr *)&their_addr, &addr_len);
	if (new_fd == -1)
	{
		std::perror("Could not create a newfd in accept()");
		return(-1);
	}
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	if(this->pollfd_count < MAXCLIENTS + 1)
	{
		poll_fd.fd = new_fd;
		poll_fd.events = POLLIN | POLLOUT;
		this->pfds.push_back(poll_fd);
	}
	else
	{
		send(new_fd, "[IRCSERV] You cannot join, the server is already full", 53, 0);
		close(new_fd);
	}
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
	std::cout << "New conection from" << s << "on socket :" << new_fd << std::endl;
	_clients.insert(std::make_pair(new_fd, new Client(new_fd)));
	_clients[new_fd]->setIPaddress(s);
	this->pollfd_count = this->pfds.size();
	return (0);
}

int Server::recieve_msg(int client_fd, int i)
{
	char buf[80];
	int readcount;

	memset(buf, 0, sizeof(buf));
	readcount = recv(client_fd, buf, sizeof(buf), 0);
	if (readcount < 0)
	{
		if (errno != EWOULDBLOCK) // no data to read
		{
			std::cerr << "Error in recv()" << std::endl;
			close_client(i, client_fd);
			return (-1);
		}
	}
	else if (readcount == 0)
	{
		std::cerr << "Peer has closed connection" << std::endl;
		close_client(i, client_fd);
		return (0);
	}
	else
	{
		setClientId(client_fd);
		setMessage(buf);
		std::cout << buf << std::endl;
		if(parseMessage(new_fd))
			return(-1);// we start parsing here
		return (0);
	}
	return (-1);
}

int Server::parseMessage(int client_fd)
{
	std::string input(_clients[client_fd]->getReadbuf());
	Message msg(input);

	switch(get_command_type(msg.command))
	{
		case command::PASS:
			if(cmd_pass(msg,client_fd))
				return(-1);
			break ;
		case command::NICK:
			if(cmd_nick(msg,client_fd))
				return(-1);
			break ;
	}
	return (0);
}

int Server::send_msg(int client_fd)
{
	std::string message;
	static int flag = 1;

	std::map<int, Client*>::iterator it;
	for(it=_clients.begin(); it!=_clients.end(); it++)
	{
		int key = it->first;
		if(key == client_fd && flag)
		{
			message = ":" + it->second->getIPaddress() + " CAP * LS ";
			send(client_fd, message.c_str(), message.length(), 0);
			flag = 0;
			return (0);
		}
		else if (key == client_fd && !flag)
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
	return (0);
}


Server::Server(std::string port, std::string password): port(port), password(password)
{
	/// check some error stuff;
	int port_number;

	port_number = std::stoi(port);
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
		if(password.empty() == true)
			throw std::exception();
	}
	catch(const std::exception& e)
	{
		std::cerr << "password is empty" << std::endl;
		return ;
	}
	try
	{
		if (serverSetup(port) < 0)
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
}

int Server::getClientId()
{
	return(this->client_id);
}

void Server::setClientId(const int id)
{
	this->client_id = id;
}

void Server::setMessage(const char* msg)
{
	std::string buf;

	std::map<int, Client*>::iterator it;
	for(it=_clients.begin(); it!=_clients.end(); it++)
	{
		int key = it->first;
		if(key == this->client_id)
		{

			buf.assign(msg);
			it->second->setReadbuf(buf);
			buf.clear();
		}
	}
}
