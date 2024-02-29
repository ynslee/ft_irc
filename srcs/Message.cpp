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
			if (token.find('\r') != std::string::npos)
			{
					token.erase(token.find('\r', 1));
				this->command = token;
				std::cout << " COMMAND ONLY: " << this->command << std::endl;
				break ;
			}
			this->command = token;
			std::cout << "COMMAND: " << this->command << std::endl;
			index ++;
		}
		else if (index >= 1  && token[0] != ':')
		{
			if (token.find('\r') != std::string::npos)
			{
				token.erase(token.find('\r', 1));
				this->params.push_back(token);
				std::cout << "PARAM WITH NO TRAILING: " << token << std::endl;
				break ;
			}
			this->params.push_back(token);
			std::cout << "PARAM: " << token << std::endl;
		}
		else if (index >= 1 && token[0] == ':')
		{
			std::string trail  = token.substr(1);
			std::string temp;
			getline(iss, temp, '\r');
			trail += temp;
			this->trailing = trail;
			std::cout << "TRAILING: " << trailing << std::endl;
		}
		if (token.empty())
			break;
	}
}

Message::~Message()
{
}
