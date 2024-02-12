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
		bool			_registerationDone;
		Client();
		Client(Client const &other);
		Client	&Client::operator=(Client const &other);
	
	public:
	Client(int new_fd);
	~Client();
	Client	&operator=(Client const &other);

	void	setSocketFd(int new_fd);
	void	setNickName(std::string nickname);
	void	setUserName(std::string _username);

	//write setters getters

	void	setSocketFd(int new_fd);
	void	setNickName(std::string new_name);
	void	setUserName(std::string new_user);
	void	setRealName(std::string new_real);


	void	setSocketFd(int new_fd);
	void	setNickName(std::string new_name);
	void	setUserName(std::string new_user);
	void	setRealName(std::string new_real);


};

#endif