/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SetServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoonslee <yoonslee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 16:31:40 by jpelaez-          #+#    #+#             */
/*   Updated: 2024/02/06 15:37:42 by yoonslee         ###   ########.fr       */
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
		std::cout << socketfd << std::endl;
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
	return (acceptPendingConnections(socketfd, their_addr));
}

//accepting pending connections
int Server::acceptPendingConnections(int socketfd, struct sockaddr_storage their_addr){
	socklen_t addr_len;
	int new_fd;
	char s[INET6_ADDRSTRLEN];

	while (1)
	{
		addr_len = sizeof(their_addr);
		new_fd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_len); // ready to communicate on socket descriptor new_fd!
		if (new_fd == -1)
		{
			// std::perror("Could not create a newfd in accept()");
			continue;
		}
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
		std::cout << "Got connected with " << s << std::endl;
		if (sendRecv(new_fd, socketfd) == -1)
			std::perror("Error in send()");
		close(new_fd);
		return (0);
		// else 
		// 	break ;
	}
	// std::cout << "Got connected with " << s << std::endl;
	std::cout << "Came at the end of server set up" << std::endl;
	free(s);
	return (0);
}

int Server::sendRecv(int new_fd, int socketfd)
{
	// std::cout << "came to sendrecv()" << std::endl;
	char buf[80];
	std::cout << "new fd: " << new_fd << std::endl;
	// std::cout << "socket fd: " << socketfd << std::endl;
	std::cerr << std::strerror(errno) << '\n';
	while (1)
	{
		int rc = recv(new_fd, buf, sizeof(buf), 0);
		// std::cout << rc << " RC" << std::endl;
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK) // no data to read
			{
				std::cerr << "Error in recv()" << std::endl;
				break ;
			}
		}
		if (rc == 0)
		{
			std::cerr << "Peer has closed connection" << std::endl;
			return (1);
		}
		if (rc > 0)
		{
			std::cout << buf << std::endl;
			int len = rc;
			rc = send(new_fd, buf, len, 0);
			if (rc == -1)
			{
				std::cerr << "Error in send()" << std::endl;
				return (-1);
			}
		}
	}
	return (-1);
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