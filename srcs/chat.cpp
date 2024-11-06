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

FileException::FileException( const char* msg ) : msg(msg) {}
FileException::~FileException( void ) throw() {}
const char * FileException::what( void ) const throw() {
	return (msg.c_str());
}