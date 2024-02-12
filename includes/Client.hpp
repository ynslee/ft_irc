#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

class Client{
	private:
		int				_client_fd;
		char*			_readbuf;
		char*			_sendbuf;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_mode;
		bool			_connection_password;
		bool			_registrationDone;
		bool			_welcomeSent;
	
	public:
	Client(int fd);
	~Client();

	//write setters getters

};

#endif