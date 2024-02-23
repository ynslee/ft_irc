#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Common.hpp"

Channel::Channel(std::string const &name) : _channel(name), _userLimit(10), _useramount(0){

	std::map<std::string, Client *>	_clientList;
	// _kickedUsers.clear();
	_operators.clear();
}

Channel::~Channel(){
	// std::map<std::string, Client *>::iterator it;
	// for (it=_clientList.begin(); it!=_clientList.end(); it++)
	// {
	// 	delete it->second;
	// }
}

std::map <std::string, Client *>&	Channel::getClientList() { return _clientList; }
std::vector<std::string>&		Channel::getChannelOperators() { return _operators; }
const std::string&				Channel::getChannelName() { return _channel; }
const std::string&				Channel::getChannelKey() { return _channelKey; }
const std::string&				Channel::getTopic() { return _topic; }
const std::string&				Channel::getMode() { return _mode; }
const int&						Channel::getUserLimit() { return _userLimit; }

void	Channel::addToChannel(Client &client)
{
	std::string nick = client.getNickName();
	_clientList.insert(std::make_pair(nick, (&client)));
	_useramount++;
}

void	Channel::removeFromChannel(std::string &nick)
{
	_clientList.erase(nick);
	removeOperator(nick);
}

void	Channel::addOperator(std::string operatorName)
{
	_operators.push_back(operatorName);
}

void	Channel::removeOperator(std::string operatorName)
{
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), operatorName);
	_operators.erase(it);
}

void	Channel::setChannelKey(std::string password)
{
	_channelKey = password;
}

void	Channel::setTopic(std::string& newTopic)
{
	_topic = newTopic;
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

bool	Channel::isOperator(std::string &operatorName)
{
	if (_operators.size() == 0)
		return(false);
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), operatorName);
	if (it == _operators.end())
		return(false);
	return(true);
}

void	Channel::removeChannelPassword()
{
	_channelKey.clear();
}


