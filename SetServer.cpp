/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SetServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoonseonlee <yoonseonlee@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:31:40 by jpelaez-          #+#    #+#             */
/*   Updated: 2024/02/08 00:32:31 by yoonseonlee      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Server::serverSetup(std::string prt, std::string password)
{
	int status, socketfd, new_fd;
	struct addrinfo hints, *p;
	struct addrinfo *serverinfo; // points to a linked list of results
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];

	// std::cout << "av[1] " << av[1] << std::endl;
	std::stringstream ss;
	ss << prt;
	std::string str = ss.str();
	const char *port = str.c_str();
	std::cout << "port " << port << std::endl;

// SETTING UP getaddrinfo()
// that does all kinds of good stuff for you,
// including DNS and service name lookups, and fills out the structs you need

	std::memset(&hints, 0, sizeof(hints));	// to empty the struct
	hints.ai_family = AF_UNSPEC;		// IPv4 or IPv6 agnostic
	hints.ai_socktype = SOCK_STREAM;	// TCP sockets
	hints.ai_flags = AI_PASSIVE;		// to fill in IP
	if ((status = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0) // error management!
	{
		std::cerr << "Error in getaddrinfo: " << gai_strerror(status) << std::endl;
		return (-1);
	}

// loop trough possible socket addresess and coonect to first possible one
// create socket: using serverinfo from get addrinfo() to call socket()
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
// set non-blocking flag and bind to an ip and port
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

// mark socket for listening IN A LOOP??
	if (listen(socketfd, MAXCLIENTS) == -1)
	{
		std::cerr << "Error in listen()" << std::endl;
		return (-1);
	}
	struct pollfd poll_fd;
	poll_fd.fd = socketfd;	
	poll_fd.events = POLLIN;
	this->pfds.push_back(poll_fd);
	std::cout << this->pfds[0].fd << std::endl;
	return (0);
}

int Server::poll_loop()
{
	int poll_count;
	this->pollfd_count =  this->pfds.size();
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
				if(this->pollfd_count < MAXCLIENTS + 1){
					if(this->pfds[i].fd == this->pfds[0].fd)
					{
						if(acceptPendingConnections())
							return(-1);
					}
					else
					{
						if(recieve_msg(this->pfds[i].fd) == -1)
							return(-1);
					}	
				}	
			}
			else if (this->pfds[i].revents & POLLOUT){
				for (int i = 0; i < this->pollfd_count; i++){
					if (this->pfds[i].fd == getClientId()){
						if (send_msg(this->pfds[i].fd) == -1){
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
	return(0);
}

int Server::acceptPendingConnections()
{
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	int new_fd;
	char s[INET6_ADDRSTRLEN];
	struct pollfd poll_fd;
	
	addr_len = sizeof(their_addr);
	new_fd = accept(this->pfds[0].fd, (struct sockaddr *)&their_addr, &addr_len); // ready to communicate on socket descriptor new_fd!
	if (new_fd == -1)
	{
		std::perror("Could not create a newfd in accept()");
		return(-1);
	}
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	poll_fd.fd = new_fd;	
	poll_fd.events = POLLIN | POLLOUT;
	this->pfds.push_back(poll_fd);
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
	std::cout << "New conection from" << s << "on socket" << new_fd << std::endl;
	this->pollfd_count = this->pfds.size();
	return (0);
}

int Server::recieve_msg(int new_fd)
{
	// std::cout << "came to sendrecv()" << std::endl;
	char buf[80];
	int readcount;

	memset(buf, 0, sizeof(buf));
	this->message.clear();
	readcount = recv(new_fd, buf, sizeof(buf), 0);
	// std::cout << rc << " RC" << std::endl;
	if (readcount < 0)
	{
		if (errno != EWOULDBLOCK) // no data to read
		{
			std::cerr << "Error in recv()" << std::endl;
			return (-1);
		}
	}
	else if (readcount == 0)
	{
		std::cerr << "Peer has closed connection" << std::endl;
		return (1);
	}
	else
	{
		std::cout << buf << std::endl;
		setMessage(buf);
		setClientId(new_fd);
		return (0);
	}
	return (-1);
}

int Server::send_msg(int new_fd)
{
	if (this->message.empty() == true)
		return (0);
	int len = this->message.length();
	const char *msg = this->message.c_str();
	int send_readcount = send(new_fd, msg, len, 0);
	if (send_readcount == -1)
	{
		// std::cerr << "Error in send()" << std::endl;
		return (-1);
	}
	std::cout << send_readcount << " bytes sent" << std::endl;
	msg = NULL;
	this->message.clear();
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
		std::cerr << e.what() << '\n';
		return ;
	}
	try
	{
		if(password.empty() == true)
			throw std::exception();
	}
	catch(const std::exception& e)
	{
		std::cerr << "password is empty" << '\n';
		return ;
	}
    try
	{
		if (serverSetup(port,password) < 0)
			throw std::runtime_error("Could not set server up");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return ;
	}
}

Server::Server(): port(), password()
{
}

Server::~Server()
{
}

const int Server::getClientId(){
	return(this->client_id);
}

void	Server::setClientId(const int id){
	this->client_id = id;
}

void    Server::setMessage(const char* msg){
	this->message = msg;
}