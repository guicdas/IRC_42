#pragma once

#include "irc.hpp"

class Client;

class Channel
{
	private:
		std::string	name;
		std::string	topic;

	public:
		std::vector< Client > operators;
		std::vector< Client > clients;

		Channel( std::string );
		Channel( Channel const & );
		Channel &operator=( Channel const & );
		~Channel( void );

	std::string getTopic( void );
	std::string getName( void );
	void setTopic( std::string );
	void setName( std::string );

	void	eraseClientFromChannel( Client & );
	int		sendMsgToChannel( std::string, std::string );
	void	addClientToChannel( Client & );
	void	addOperatorToChannel( Client & );
	void	listAllMembersInChannel( Client & );

	int		isClientInChannel( std::string );
	int		isOperatorInChannel( Client & );

	void	checkClientInChannel( Client & );
};