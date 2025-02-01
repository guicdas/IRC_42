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

std::string	Client::getUser( void ){
	return (this->username);
}

std::string	Client::getRealname( void ){
	return (this->realname);
}

std::string	Client::getId( void ){
	return (this->id);
}

bool Client::getRegisterd( void ){
	return (this->registered);
}

int	Client::getFd( void ){
	return (this->fd);
}

void	Client::setNick( std::string nick ){
	this->nickname = nick;
}

void	Client::setUser( std::string user ){
	this->username = user;
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

std::string Client::resolveHostname( int socketFd ){
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	if (getpeername(socketFd, (struct sockaddr*)&addr, &addr_len) == -1)
		throw("ERROR: Failed to get host name!");

	const char* ip = inet_ntoa(addr.sin_addr);

	struct hostent* host_entry = gethostbyname(ip);
	if (host_entry && host_entry->h_name)
		return (host_entry->h_name);
	return (ip);
	
}

std::string	Client::createId( void ){
	std::string _id = nickname + "!" + username + "@" + this->resolveHostname(this->getFd());
	return _id;
}