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
	this->socket = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->socket == -1)
	{
		std::cerr << "Error: creating socket: ";
		throw(FileException(strerror(errno)));
	}
	int reuse = 1;
	if (setsockopt(this->socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
	{
		close(this->socket);
		std::cerr << "Error: setsockopt(): ";
		throw(FileException(strerror(errno)));
	}
}

void	Chat::prepareServerSocket( void ){
	std::memset(&this->serverAddr, 0, sizeof(this->serverAddr));
		serverAddr.sin6_family = AF_INET6;
		serverAddr.sin6_port = htons(port);
		serverAddr.sin6_addr = in6addr_any;

		/* The bind() function binds a unique local name 
		to the socket with descriptor socket. After calling socket(), a 
		descriptor does not have a name associated with it. However, 
		it does belong to a particular address family as specified when socket() is called.
		The exact format of a name depends on the address family. */
		if (bind(this->socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		{
			close(this->socketFd);
			std::cerr << "Error: bind(): ";
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
}

int	Chat::getServerFd( void ){
	return (this->socketFd);
}

unsigned int	Chat::getServerPort( void ){
	return (this->port);
}

FileException::FileException( const char* msg ) : msg(msg) {}
FileException::~FileException( void ) throw() {}
const char * FileException::what( void ) const throw() {
	return (msg.c_str());
}
/*
void Chat::SignalHandler( int signum )
{
	std::cout << std::endl << "Signal Received: " << signum << std::endl;
	Chat::Signal = true; //-> set the static boolean to true to stop the server
}

void Chat::CloseFds( void ){
	for(size_t i = 0; i < clients.size(); i++){ 
		std::cout << "Client <" << clients[i].GetFd() << "> Disconnected" << std::endl;
		close(clients[i].GetFd());
	}
	if (SerSocketFd != -1){ //-> close the Chat socket
		std::cout << "Chat <" << SerSocketFd << "> Disconnected" << std::endl;
		close(SerSocketFd);
	}
}*/