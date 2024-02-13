#include "../includes/Message.hpp"

Message::Message(std::string input)
{
	std::string token;
	std::stringstream iss(input);

	while (iss >> token)
	{
		std::cout << token << std::endl;
	}

	// basic parsing here
}

Message::~Message()
{
}
