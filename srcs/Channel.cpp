#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Common.hpp"
#include <cstdlib>
#include <ctime>

Channel::Channel(std::string const &name) : _channel(name), _mode("+nt"), _userLimit(10), _useramount(0)
{

	std::map<std::string, Client *>	_clientList;
	std::srand(std::time(0));
	int randomise = rand();
	if (randomise % 2 == 0)
		_topic = "Welcome to 🐾TYCHUNEN SERVER🐾 support | Not a channel for politics, drama, fights | FAQ and guides: please ask Pets around you | Usable commands: MODE, INVITE, QUIT, PRIVMSG, KICK, TOPIC" ;
	_operators.clear();
	std::cout << "Channel " << GREEN << _channel << RESET << " created" << std::endl;
}

Channel::~Channel() {}

std::map <std::string, Client *>&	Channel::getClientList() { return _clientList; }

std::vector<std::string>&	Channel::getChannelOperators() { return _operators; }

std::vector<std::string>&		Channel::getInvitedList() { return _invitedList; }
const std::string&	Channel::getChannelName() { return _channel; }

const std::string&	Channel::getChannelKey() { return _channelKey; }

const std::string&	Channel::getTopic() { return _topic; }

const std::string&	Channel::getMode() { return _mode; }

unsigned int&	Channel::getUserLimit() { return _userLimit; }

void	Channel::addToChannel(Client &client)
{
	std::string nick = client.getNickName();
	_clientOrder.push_back(&client);
	_clientList.insert(std::make_pair(nick, (&client)));
	_useramount++;
}

void	Channel::removeFromChannel(const std::string &nick)
{
	std::vector<std::string>::iterator iter2 = std::find(_invitedList.begin(), _invitedList.end(), nick);
    if(iter2 != _invitedList.end())
    	_invitedList.erase(iter2);
	_clientList.erase(nick);
	removeOperatorQuit(nick);
	_useramount--;
}

void	Channel::addOperator(std::string clientNickName)
{
	if (_operators.size() == 0)
	{
		_operators.push_back(clientNickName);
		return ;
	}
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), clientNickName);
	if(it == _operators.end())
	{
		_operators.push_back(clientNickName);
	}
}

void	Channel::removeOperatorQuit(std::string clientNickName)
{
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), clientNickName);
	if(it != _operators.end())
	{
		_operators.erase(it);
		std::vector<Client*>::iterator iter;
		for(iter = _clientOrder.begin(); iter != _clientOrder.end(); iter++)
		{
			if((*iter)->getNickName() == clientNickName)
			{
				_clientOrder.erase(iter);
				break;
			}
		}
		if(_operators.empty() == true && _clientOrder.size() >= 1)
		{
			std::string nickname = _clientOrder.front()->getNickName();
			_operators.push_back(nickname);
		}
	}
	else
	{
		std::vector<Client*>::iterator iter2;
		for(iter2 = _clientOrder.begin(); iter2 != _clientOrder.end(); iter2++)
		{
			if((*iter2)->getNickName() == clientNickName)
			{
				_clientOrder.erase(iter2);
				break;
			}
		}
	}
}

void	Channel::removeOperator(std::string clientNickName)
{
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), clientNickName);
	if(it != _operators.end())
	{
		_operators.erase(it);
		if(_operators.empty() == true && _clientOrder.size() > 1)
		{
			_operators.push_back(_clientOrder.front()->getNickName());
		}
	}
	return ;
}


void	Channel::setChannelKey(std::string password)
{
	_channelKey = password;
}

void	Channel::setTopic(std::string& newTopic)
{
	_topic = newTopic;
}

void	Channel::setMode(std::string mode, Client *client)
{
	if (goodModeFLag(mode) == false)
	{
		send(client->getClientFd(), ERR_UNKNOWNMODE(), strlen(ERR_UNKNOWNMODE()), 0);
		return ;
	}
	char modeFlag = mode[1];
	if (mode[0] == '+' && isChannelFlag(mode) == false)
		_mode += modeFlag;
	if (mode[0] == '-' && isChannelFlag(mode) == true)
			_mode.erase(std::remove(_mode.begin(), _mode.end(), modeFlag), _mode.end());
}

void	Channel::setUserLimit(unsigned int newLimit)
{

	if (newLimit <= 100)
		_userLimit = newLimit;
}

void	Channel::addMode(std::string const mode)
{
	if (_mode.empty() == true)
		_mode = "+" + mode;
	else
		_mode += mode;
}

void	Channel::removeMode(std::string const mode)
{
	size_t pos = 0;

	pos = _mode.find(mode);
	if (pos != 0)
		_mode.erase(pos, mode.length());
}

bool	Channel::isAlreadyInChannel(std::string &nick)
{
	if (_clientList.size() == 0)
		return(false);
	std::map<std::string, Client *>::iterator it;
	for (it=_clientList.begin(); it!=_clientList.end(); it++)
	{
		if (it->first == nick)
			return(true);
	}
	return(false);
}

bool	Channel::isOperator(const std::string &clientNickName)
{
	if (_operators.size() == 0)
		return(false);
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), clientNickName);
	if (it == _operators.end())
		return(false);
	return(true);
}

void	Channel::removeChannelPassword()
{
	_channelKey.clear();
}

bool Channel::goodModeFLag(std::string modeFlag)
{
	if (modeFlag.size() != 2)
		return false;
	else if (modeFlag[0] != '+' && modeFlag[0] != '-')
		return false;
	else if (modeFlag[1] != 'i' && modeFlag[1] != 't' && modeFlag[1] != 'k' && modeFlag[1] != 'o' && modeFlag[1] != 'l')
		return false;
	return true;
}

bool Channel::isChannelFlag(std::string flag)
{
	std::string currentMode = this->getMode();
	size_t pos = 0;
	pos = currentMode.find(flag[1]);
	if (pos != 0 && pos != std::string::npos)
		return true;
	return false;
}
