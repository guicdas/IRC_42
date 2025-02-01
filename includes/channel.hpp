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

};