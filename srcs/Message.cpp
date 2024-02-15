#include "../includes/Message.hpp"

Message::Message(std::string input)
{
	std::string token;
	std::stringstream iss(input);
	int index = 0;

	while (iss >> token)
	{
		if (index == 0)
		{
			this->command = token;
			std::cout << "COMMAND: " << this->command << std::endl;
		}
		else if (index >= 1  && token[0] != ':')
		{
			this->params.push_back(token);
			std::cout << "PARAM: " << token << std::endl;
		}
		else if (index >= 1 && token[0] == ':')
		{
			std::string trailing  = token.substr(1);
			std::string temp;
			getline(iss, temp, '\n');
			trailing += temp;
			std::cout << "TRAIILING: " << trailing << std::endl;
		}
		index ++;
	}
}

Message::~Message()
{
}
