#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return -1;
	}
	Server New_server(argv);

	// Should we pulling here?? 
	return (0);
}
