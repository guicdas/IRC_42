#include "../includes/irc.hpp"

Client::Client(int clientSocket) : 
	fd(clientSocket), 
	nickname(""), 
	realname(""), 
	username(""),
    registered(false), 
	buffer(""),
	channels(),
	args()
{
	if (clientSocket < 0)
		PRINT_COLOR(RED, "Invalid client socket!");
}


Client::Client(const Client &c) 
{
    this->fd = c.fd;
    this->nickname = c.nickname;
    this->realname = c.realname;
    this->username = c.username;
    this->registered = c.registered;
    this->channels = c.channels;
    this->args = c.args;
    this->buffer = c.buffer;
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

