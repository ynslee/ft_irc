#include "Server.hpp"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int Server::serverSetup(char **av)
{
	int status, socketfd, new_fd;
	struct addrinfo hints, *p;
	struct addrinfo *serverinfo; // points to a linked list of results
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	int yes = 1;

	// std::cout << "av[1] " << av[1] << std::endl;
	std::stringstream ss;
	ss << av[1];
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
			std::perror("Could not create a newfd in accept()");
			continue;
		}
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
		std::cout << "Got connected with " << s << std::endl;
		// if (!fork())
		// 	close(socketfd);
		if (send(new_fd, "Hello, world!", 13, 0) == -1)
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


Server::Server(char **argv)
{
    /// check some error stuff;
    try
	{
		if (serverSetup(argv) < 0)
			std::cerr << "Could not set server up" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}