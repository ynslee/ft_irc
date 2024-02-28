#include "../includes/Client.hpp"
#include "../includes/Reply.hpp"

Client::Client(){};

/**
 * @brief Construct a new Client:: Client object
 *
 * @param new_fd socket fd
 * @param _mode mode for USER, CHANNEL
 */
Client::Client(int new_fd): _clientFd(new_fd), _serverName("🐾TYCHUNEN SERVER🐾"), _nickname(""), _mode("+Ziw"), _operatorPassword("TYCHUNEN"),_isRegistered(0), _welcomeSent(0), _isOperator(false), _maxChannels(0)
{
	std::vector<std::string>	_channelsJoined;
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
	_clientFd = new_fd;
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
	if (goodModeFLag(mode) == false)
		send(this->getClientFd(), ERR_UNKNOWNMODE(), strlen(ERR_UNKNOWNMODE()), 0);
	std::cout << "**** OLD MODE: " << _mode << std::endl;
	char modeFlag = mode[1];
	if (mode[0] == '+')
		_mode += modeFlag;
	if (mode[0] == '-')
		_mode.erase(std::remove(_mode.begin(), _mode.end(), modeFlag), _mode.end());
	std::cout << "**** NEW MODE: " << _mode << std::endl;
}

void	Client::setIPaddress(std::string IP)
{
	_IPaddress = IP;
}

void	Client::setHostName(char *host)
{
	_hostname.assign(host);
}

void	Client::setRegisteration(int reg)
{
	_isRegistered = reg;
}

void	Client::setWelcomeSent(int sent)
{
	_welcomeSent = sent;
}
void	Client::setIsOperator(bool status)
{
	_isOperator = status;
}

void	Client::setNewChannel(std::string channel_name)
{
		_channelsJoined.push_back(channel_name);
}

void	Client::setMaxChannels(void)
{
	_maxChannels++;
}

const int	&Client::getClientFd(void){return(_clientFd);}
const std::string	&Client::getServerName(void){return(_serverName);}
const std::string	&Client::getNickName(void){return(_nickname);}
const std::string	&Client::getUserName(void){return(_username);}
const std::string	&Client::getRealName(void){return(_realname);}
const int	&Client::getSocketFd(void){return(_clientFd);}
const std::string	&Client::getReadbuf(void){return(_readbuf);}
const std::string	&Client::getSendbuf(void){return(_sendbuf);}
const std::string	&Client::getIPaddress(void){return(_IPaddress);}
const std::string	&Client::getHostName(void){return(_hostname);}
const std::string	&Client::getMode(void){return(_mode);}
const std::string	&Client::getOperatorPassword(void){return(_operatorPassword);}
const int	&Client::getRegisteration(void){return(_isRegistered);}
const int &Client::getWelcomeSent(void){return(_welcomeSent);}
const bool &Client::getOperatorStatus(void){return(_isOperator);}
const int	&Client::getMaxChannels(void){return(_maxChannels);}
std::vector<std::string> &Client::getChannelsJoined(void){return(this->_channelsJoined);}

bool Client::goodModeFLag(std::string modeFlag)
{
	std::string hostname = this->getHostName();
	if (modeFlag.size() != 2)
		return false;
	else if (modeFlag[0] != '+' && modeFlag[0] != '-')
		return false;
	else if (modeFlag[1] != 'i' && modeFlag[1] != 'k' && modeFlag[1] != 'o' && modeFlag[1] != 'l')
		return false;
	return true;
}

void	Client::addSendbuf(std::string buf)
{
	_sendbuf += buf;
}
