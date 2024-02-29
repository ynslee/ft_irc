#include "../includes/Message.hpp"
#include <cctype>
#include <cstring>

Message::Message(std::string input)
{
	std::string token;
	std::stringstream iss(input);
	int index = 0;

	while (iss >> token)
	{
		if (index == 0)
		{
			for (int i = 0; i < static_cast<int>(token.length()); i++)
				token[i] = std::toupper(token[i]);
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
			std::string trail = token.substr(1);
			std::string temp;
			getline(iss, temp, '\n');
			trail += temp;
			this->trailing = trail;
			std::cout << "TRAILING: " << trailing << std::endl;
		}
		index ++;
	}
}

Message::~Message()
{
}
