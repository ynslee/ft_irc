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
		bool			_password;
		bool			_registrationDone;
		bool			_welcomeSent;
	
	public:
	Client();
	Client(int fd);
	Client(Client const& other);
	~Client();
	Client	&operator=(Client const &other);

	void	setSocketFd(int new_fd);
	void	setNickName(std::string nickname);
	void	setUserName(std::string _username);

	//write setters getters

};

#endif