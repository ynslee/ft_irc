#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define MAXCLIENTS 32

int serverSetup(char **av)
{
	int status, socketfd, new_fd;
	struct addrinfo hints;
	struct addrinfo *serverinfo; // points to results
	struct sockaddr_storage their_addr;
	socklen_t addr_len;

	std::stringstream ss;
	ss << av[1];
	std::string str = ss.str();
	const char *port = str.c_str();

// SETTING UP getaddrinfo()
// that does all kinds of good stuff for you,
// including DNS and service name lookups, and fills out the structs you need

	memset(&hints, 0, sizeof(hints));	// to empty the struct
	hints.ai_family = AF_UNSPEC;		// IPv4 or IPv6 agnostic
	hints.ai_socktype = SOCK_STREAM;	// TCP sockets
	hints.ai_flags = AI_PASSIVE;		// to fill in IP
	if ((status = getaddrinfo(NULL, port, &hints, &serverinfo)) != 0) // error management!
	{
		std::cerr << "Error in getaddrinfo: " << gai_strerror(status) << std::endl;
		return (-1);
	}

// create socket: using serverinfo from get addrinfo() to call socket()
	socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol); //ERROR HANDLING??

// bind to an ip and port
	bind(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen); //ERROR HANDLING??

// mark socket for listening
	listen(socketfd, MAXCLIENTS);

// accept pending connections
	addr_len = sizeof(their_addr);
	new_fd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_len); // ready to communicate on socket descriptor new_fd!

	std::cout << "Came at the end of server set up" << std::endl;
	freeaddrinfo(serverinfo);
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
		if (serverSetup(argv) != 0)
			std::cerr << "Could not set server up" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
