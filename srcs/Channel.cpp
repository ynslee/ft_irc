#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

Channel::Channel(std::string const &name) : _channel(name), _userLimit(10){
	_clientList.clear();
	_kickedUsers.clear();
	_operators.clear();
}

Channel::~Channel(){}

const std::string&				Channel::getChannelName() { return _channel; }
const std::string&				Channel::getTopic() { return _topic; }
const std::string&				Channel::getMode() { return _mode; }
