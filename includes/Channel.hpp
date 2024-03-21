#pragma once

#include "Common.hpp"
#include "Client.hpp"

class Client;

class Channel
{
	private:
		std::map<std::string, Client *>	_clientList;
		std::vector<std::string>		_operators;
		std::vector<std::string>		_invitedList;
		std::vector<Client *>           _clientOrder;
		std::string 					_channel;
		std::string						_channelKey;
		std::string						_topic;
		std::string						_mode;
		unsigned int					_userLimit;
		int								_useramount;

	public:
		Channel(std::string const &name);
		~Channel();

		/*getters and setters*/
		std::map <std::string, Client *>	&getClientList();
		std::vector<std::string>&		getChannelOperators();
		std::vector<std::string>&		getInvitedList();
		const std::string&				getChannelName();
		const std::string&				getChannelKey();
		const std::string&					getTopic();
		const std::string&				getMode();
		unsigned int&							getUserLimit();
		void							addToChannel(Client &client);
		void							removeFromChannel(const std::string &nick);
		void							addOperator(std::string clientNickName);
		void							removeOperator(std::string operatoName);
		void							removeOperatorQuit(std::string clientNickName);
		void							setChannelKey(std::string password);
		void							setTopic(std::string& newTopic);
		void							setMode(std::string mode, Client *client);
		void							setUserLimit(unsigned int newLimit);
		void							addMode(std::string const mode);
		void							removeMode(std::string const mode);

		/*helping functions*/
		bool							isAlreadyInChannel(std::string &nick);
		bool							isOperator(const std::string &operatorName);
		void							removeChannelPassword();
		bool 							goodModeFLag(std::string modeFlag);
		bool							isChannelFlag(std::string flag);
};
