#include "../includes/irc.hpp"

Client::Client( int	clientSocket ){
	this->fd = clientSocket;
	this->buffer = "";
	this->args =	std::vector<std::string>();
	this->nickname =	"";
	this->realname =	"";
	this->username =	"";
	this->channels = std::vector< Channel >();
	this->registered = 0;
}

Client::Client( Client const &c ){
	*this = c;
}

Client	&Client::operator=( Client const &c ){
	if (this == &c)
		return (*this);
	return (*this);
}

Client::~Client( void ){}




std::string	Client::getNick( void ){
	return (this->nickname);
}

int	Client::getFd( void ){
	return (this->fd);
}

void	Client::setUser( std::string name ){
	this->username = name;
}

void	Client::setRealname( std::string name ){
	this->realname = name;
}


void	Client::verifyClientRegistered( void ){
	if (this->registered == 0)
		throw (462); //mudar numero
}

void	Client::verifyValidNick( void ){
	std::string	forbiddenChars[11] = {",*.?!@\"$# "};

	// falta o ':' que so pode estar em primeiro
	if (std::strpbrk(this->args.at(1).c_str(), forbiddenChars->c_str()) != NULL)
		throw (432);
}

