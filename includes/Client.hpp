#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Common.hpp"

class Client{
	private:
 
	
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