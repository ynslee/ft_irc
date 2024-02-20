#pragma once

#include "Common.hpp"
#include "Client.hpp"

class Client;


//userlimit = user capacity limits in channel
class Channel
{
	private:
		std::map<std::string, Client>	_clientList;
		std::vector<std::string>		_kickedUsers;
		std::vector<std::string>		_operators;
		std::string						_operatorPassword;
		std::string 					_channel;
		std::string						_channelKey;
		std::string						_topic;
		std::string						_mode;
		int								_userLimit;
		 
	public:
		Channel(std::string const &name);
		~Channel();


		const std::string&				getChannelName();
		const std::string&				getTopic() ;
		const std::string&				getMode() ;
		const std::string&				getChannelKey() ;
		const int&						getUserLimit() ;
		std::vector<std::string>&		getOperators() ;
		std::vector<std::string>&		getKickedUsers() ;
		std::map <std::string, Client>&	getClientList();
		void							setTopic(std::string& newTopic);
		void							setChannelKey(std::string password);
		void							setUserLimit(int limit);
		void							addToChannel(Client &client);
		void							removeFromChannel(std::string &nick);
		void							addOperator(std::string operatorName);
		void							removeOperator(std::string operatoName);
		bool							isOperator(std::string &operatorName);
		void							addMode(std::string const mode);
		void							removeMode(std::string const mode);
		void							removeChannelPassword();
		void							addToKicked(std::string &banned_name);

};
