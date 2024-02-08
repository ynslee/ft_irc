
#include "Client.hpp"

Client::Client(int new_fd): _client_fd(new_fd){
		std::string		_readbuf;
		std::string		_sendbuf;
		bool			_to_deconnect;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_mode;
		bool			_connection_password;
		bool			_registrationDone;
		bool			_welcomeSent;
}