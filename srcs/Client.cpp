#include "../includes/Client.hpp"

Client::Client(){};

/**
 * @brief Construct a new Client:: Client object
 * 
 * @param new_fd socket fd
 * @param _mode mode for USER, CHANNEL
 */
Client::Client(int new_fd): _client_fd(new_fd), _readbuf(NULL), _sendbuf(NULL){ 
		_registerationDone = false;
		_to_deconnect = false;
}

Client::~Client(){
	if(_readbuf)
		delete(_readbuf);
	if(_sendbuf)
		delete(_sendbuf);
}

Client::Client(Client const &other){
	*this = other;
}

//What do we want to copy using assignment operator?
Client	&Client::operator=(Client const &other){
	if (this != &other){
		_nickname = other._nickname;
		_username = other._username;
		_readbuf = other._readbuf;
		_sendbuf = other._sendbuf;
		_mode = other._mode;
	}
}

void	Client::setSocketFd(int new_fd){
	_client_fd = new_fd;
}

void	Client::setNickName(std::string new_name)
{
	_nickname = new_name;
}
void	setUserName(std::string new_user);
void	setRealName(std::string new_real);