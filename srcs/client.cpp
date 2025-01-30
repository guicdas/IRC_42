#include "../includes/irc.hpp"

Client::Client( int	clientSocket ){
	this->fd = clientSocket;
	this->buffer = "";
	this->args = std::vector<std::string>();
	this->nickname = "";
	this->realname = "";
	this->username = "";
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


std::string	Client::getRealname( void ){
	return (this->realname);
}

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

void	Client::setHostname(const std::string& hostname)
{
	if (!hostname.empty())
		this->hostname = hostname;
}

void	Client::setId()
{
	if (this->username != "" && this->hostname != "" && this->nickname != "")
		this->id = this->nickname + "!" + this->username + "@" + this->hostname;
}

void Client::resolveHostname(int _socketFd)
{
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	if (getpeername(_socketFd, (struct sockaddr*)&addr, &addr_len) == -1)
		throw (FileException("ERROR: Failed to get host name!"));

	const char* ip = inet_ntoa(addr.sin_addr);

	struct hostent* host_entry = gethostbyname(ip);
	if (host_entry && host_entry->h_name)
		this->setHostname(host_entry->h_name);
	else
		this->setHostname(ip);
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

