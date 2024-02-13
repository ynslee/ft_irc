#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

// if _registeration == 0, then the client is not registered
// if _registeration == 1, then the client pass is registered
// if _registeration == 2, then the client pass and nick are registered
// if _registeration == 3, then the client pass, nick and user are registered
class Client{
	private:
		int				_client_fd;
		std::string		_readbuf;
		std::string		_sendbuf;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_mode;
		std::string		_IPaddress;
		int				_isRegistered;
		Client();
		Client(Client const &other);
		Client	&operator=(Client const &other);

	public:
		Client(int new_fd);
		~Client();
		//write setters getters

		void	setSocketFd(int new_fd);
		void	setNickName(std::string new_name);
		void	setUserName(std::string new_user);
		void	setRealName(std::string new_real);
		void	setReadbuf(std::string buf);
		void	setSendbuf(std::string buf);
		void	setMode(std::string mode);
		void	setIPaddress(char *ip);
		void	setRegisteration(int reg);
		const std::string	&getNickName(void);
		const std::string	&getUserName(void);
		const std::string	&getRealName(void);
		const int	&getSocketFd(void);
		const std::string	&getReadbuf(void);
		const std::string	&getSendbuf(void);
		const std::string	&getIPaddress(void);
		const std::string	&getMode(void);
		const int	&getRegisteration(void);
};

#endif
