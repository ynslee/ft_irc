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
				return ;
			}
			else
			{
				this->command = token;
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
				return ;
			}
			this->params.push_back(token);
			continue ;
		}
		else if (index >= 1 && token[0] == ':' && command.empty() == false)
		{
			std::string trail = token.substr(1);
			std::string temp;
			getline(iss, temp, '\r');
			trail += temp;
			this->trailing = trail;
			trailing_flag = 1;
		}
	}
}

Message::~Message()
{
}
