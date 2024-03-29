#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Common.hpp"

class Message {
	private:
		Message();
		Message(Message const &other);
		Message &operator=(Message const &other);

	public:
		Message(std::string input);
		std::string	command;
		int command_flag;
		std::string	trailing;
		int trailing_flag;
		std::vector<std::string> params;
		~Message();
} ;

#endif
