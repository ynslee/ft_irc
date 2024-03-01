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
				size_t pos = token.find('\r');
				token = token.substr(0, pos);
				this->command = token;
				std::cout << " COMMAND ONLY: " << this->command << std::endl;
				return ;
			}
			else
			{
				this->command = token;
				std::cout << "COMMAND: " << this->command << std::endl;
				index ++;
				continue ;
			}
		}
		else if (index >= 1  && token[0] != ':' && command.empty() == false)
		{
			if (token.find('\r') != std::string::npos)
			{
				size_t pos = token.find('\r');
				token = token.substr(0, pos);
				this->params.push_back(token);
				std::cout << "PARAM WITH NO TRAILING: " << token << std::endl;
				return ;
			}
			this->params.push_back(token);
			std::cout << "PARAM: " << token << std::endl;
			continue ;
		}
		else if (index >= 1 && token[0] == ':' && command.empty() == false)
		{
			std::string trail = token.substr(1);
			std::string temp;
			getline(iss, temp, '\r');
			trail += temp;
			this->trailing = trail;
			std::cout << "TRAILING: " << trailing << std::endl;
		}
	}
}

Message::~Message()
{
}
