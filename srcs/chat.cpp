#include "../includes/irc.hpp"

Chat::Chat( void ){}

Chat::Chat( char **av ){
	this->password = av[2];
	this->port = std::atoi(av[1]);
}

Chat::Chat( Chat const &c ){
	*this = c;
}

Chat	&Chat::operator=( Chat const &c ){
	if (this == &c)
		return (*this);
	return (*this);
}

Chat::~Chat( void ){}

void	Chat::createServerSocket( void ){
	this->socketFd = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->socketFd == -1)
	{
		std::cerr << "Error: creating socket: ";
		throw(FileException(strerror(errno)));
	}
	int reuse = 1;
	if (setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
	{
		close(this->socketFd);
		std::cerr << "Error: setsockopt(): ";
		throw(FileException(strerror(errno)));
	}
	if (fcntl(this->socketFd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(this->socketFd);
		std::cerr << "Error: fctnl(): ";
		throw(FileException(strerror(errno)));
	}
	
	std::memset(&this->serverAddr, 0, sizeof(this->serverAddr));
	serverAddr.sin6_family = AF_INET6;
	serverAddr.sin6_port = htons(this->port);
	serverAddr.sin6_addr = in6addr_any;

	/* The bind() function binds a unique local name to the socket with descriptor socket. */
	if (bind(this->socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
	{
		close(this->socketFd);
		std::cerr << "Error: bind(): "<< this->socketFd << " ";
		throw (FileException(strerror(errno)));
	}
	/* transforms an active socket into a passive socket, Once called, 
	socket can never be used as an active socket to initiate connection requests
	It indicates a readiness to accept client connection requests, 
	and creates a connection request queue of length backlog to queue incoming 
	connection requests. Once full, additional connection requests are rejected.*/
	if (listen(this->socketFd, 5) == -1)
	{
		close(this->socketFd);
		std::cerr << "Error: listen(): ";
		throw (FileException(strerror(errno)));
	}
	//this->poll ;
}

int	Chat::getServerFd( void ){
	return (this->socketFd);
}

unsigned int	Chat::getServerPort( void ){
	return (this->port);
}

bool Chat::signal = false;
void Chat::signalHandler( int signum )
{
	std::cout << std::endl << "Signal Received: " << signum << std::endl;
	Chat::signal = true; //-> set the static boolean to true to stop the server
}

FileException::FileException( const char* msg ) : msg(msg) {}
FileException::~FileException( void ) throw() {}
const char * FileException::what( void ) const throw() {
	return (msg.c_str());
}

