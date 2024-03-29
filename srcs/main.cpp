
#include "../includes/Server.hpp"

bool serverShutdown = false;

void sig_handler(int signal)
{

	if (signal == SIGINT)
	{
		serverShutdown = true;
	}
}


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return -1;
	}
	signal(SIGINT, sig_handler);
	Server New_server(argv[1], argv[2]);
	try
	{
		if (New_server.pollLoop() < 0)
			throw std::runtime_error("Poll");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
