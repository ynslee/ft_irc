#include "../includes/Client.hpp"

Client::Client(){};

/**
 * @brief Construct a new Client:: Client object
 * 
 * @param new_fd socket fd
 * @param _mode mode for USER, CHANNEL
 */
Client::Client(int new_fd): _client_fd(new_fd)
{ 
	_registerationDone = false;
}

Client::~Client(){}

Client::Client(Client const &other)
{
	*this = other;
}

Client	&Client::operator=(Client const &other)
{
	if (this != &other){
		_nickname = other._nickname;
		_username = other._username;
		_mode = other._mode;
	}
	return (*this);
}

void	Client::setSocketFd(int new_fd)
{
	_client_fd = new_fd;
}

void	Client::setNickName(std::string new_name)
{
	_nickname = new_name;
}
void	Client::setUserName(std::string new_user)
{
	_username = new_user;
}
void	Client::setRealName(std::string new_real)
{
	_realname = new_real;
}

void 	Client::setReadbuf(std::string buf)
{
	_readbuf = buf;
}

void	Client::setSendbuf(std::string buf)
{
	_sendbuf = buf;
}

void	Client::setMode(std::string mode)
{
	_mode = mode;
}

void	Client::setIPaddress(char *ip)
{
	_IPaddress.assign(ip);
}

const std::string	&Client::getNickName(void){return(_nickname);}
const std::string	&Client::getUserName(void){return(_username);}
const std::string	&Client::getRealName(void){return(_realname);}
const int	&Client::getSocketFd(void){return(_client_fd);}
const std::string	&Client::getReadbuf(void){return(_readbuf);}
const std::string	&Client::getSendbuf(void){return(_sendbuf);}
const std::string	&Client::getIPaddress(void){return(_IPaddress);}
const std::string	&Client::getMode(void){return(_mode);}