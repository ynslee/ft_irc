#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define MAXCLIENTS 32

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int serverSetup(char **av)
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
		if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1))
			continue ;
		// std::cout << "socket: " << strerror(errno) << std::endl;
		std::cout << socketfd << std::endl;
// to manage error “Address already in use.”
		if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes) == -1))
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

// accept pending connections
	while (1)
	{
		addr_len = sizeof(their_addr);
		new_fd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_len); // ready to communicate on socket descriptor new_fd!
		if (new_fd == -1)
		{
			std::cout << "Could not create a newfd in accept()" << std::endl;
			continue ;
		}
	}
	inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));
	std::cout << "Got connected with " << s << std::endl;
	std::cout << "Came at the end of server set up" << std::endl;
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return -1;
	}
	try
	{
		if (serverSetup(argv) < 0)
			std::cerr << "Could not set server up" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
