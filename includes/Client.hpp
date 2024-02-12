#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

class Client{
	private:
		int				_client_fd;
		std::string		_readbuf;
		std::string		_sendbuf;
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
		//write setters getters

		void	setSocketFd(int new_fd);
		void	setNickName(std::string new_name);
		void	setUserName(std::string new_user);
		void	setRealName(std::string new_real);
		void	setReadbuf(std::string buf);
		const std::string	&getNickName(void);
		const std::string	&getUserName(void);
		const std::string	&getRealName(void);
		const int	&getSocketFd(void);
};

#endif