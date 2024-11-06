#include "../includes/irc.hpp"

Chat::Chat( void ){}

Chat::Chat( Chat const &c ){
	*this = c;
}

Chat	&Chat::operator=( Chat const &c ){
	if (this == &c)
		return (*this);
	return (*this);
}

Chat::~Chat( void ){}