#include "../includes/irc.hpp"
#include "../includes/fd.hpp"

Server::Server( void ){}

Server::Server( char **av ){
	this->password = av[2];
	this->port = std::atoi(av[1]);
}

Server::Server( Server const &c ){
	*this = c;
}

Server	&Server::operator=( Server const &c ){
	if (this == &c)
		return (*this);
	return (*this);
}

Server::~Server( void ){}

void	Server::acceptClient( t_env *e ){
	int	i = 0;
	/* extract the first connection on the queue of pending connections, create a 
	new socket with the same socket type protocol and address family as the specified 
	socket, and allocate a new file descriptor for that socket */
	this->clientSocket = accept(this->serverSocket, (struct sockaddr*)&this->clientAddr, (socklen_t *)sizeof(clientAddr));
	std::cout << "New client #" << this->clientSocket << " from " 
	<< inet_ntoa(this->clientAddr.sin_addr) << ":" << ntohs(this->clientAddr.sin_port) << "\n";

	while (i < e->maxFd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
	e->fds[this->clientSocket].type = 2;
	e->fds[this->clientSocket].clientRead = &Server::clientRead;
	e->fds[this->clientSocket].clientWrite = &Server::clientWrite;
}

void	Server::createServerSocket( t_env *e ){
	struct protoent	*pe;
	int reuse = 1;
	int nRequests = 10;

	pe = (struct protoent*) getprotobyname("tcp");
	this->serverSocket = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (this->serverSocket == -1)
		throw(FileException("Error: creating socket: "));
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
		throw(FileException("Error: setsockopt(): "));
	if (fcntl(this->serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(FileException("Error: fctnl(): "));
	
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(this->port);
	this->serverAddr.sin_addr.s_addr = INADDR_ANY;
	// The bind() function binds a unique local name to the socket with descriptor socket.
	if (bind(this->serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw (FileException("Error: bind(): "));

	// Once called, socket can never be used as an active socket to initiate connection requests 
	// readiness to accept client connection requests, and creates a connection 
	// request queue of length backlog to queue incoming connection requests. 
	// Once full, additional connection requests are rejected.
	if (listen(this->serverSocket, nRequests) == -1)
		throw (FileException("Error: listen(): "));
	//if (!&fds[this->clientSocket])
	//	throw ();
	e->fds[this->serverSocket].type = 1;
	e->fds[this->serverSocket].clientRead = &Server::acceptClient;
}

void	Server::loop( t_env *e ){
	int	i = 0;
	int	maxFds = 0;
	int	nFds;

	FD_ZERO(&this->fdRead);
	FD_ZERO(&this->fdWrite);
	//FD_ZERO(&this->fdExcep);
	while (i < e->maxFd)
	{
		if (e->fds[i].type != FREE)
		{
			FD_SET(i, &this->fdRead);
			if (strlen(e->fds[i].writeBuf) > 0)
				FD_SET(i, &this->fdWrite);
			maxFds = (maxFds > i) ? maxFds : i;
		}
		i++;
	}
	nFds = select(maxFds + 1, &this->fdRead, &this->fdWrite, NULL, NULL);	//timeval
	i = 0;
	while ((i < e->maxFd) && (nFds > 0))
	{
		if (!e->fds[i].clientRead)
			throw (FileException("deu merda"));
		if (FD_ISSET(i, &this->fdRead))
			e->fds[i].clientRead(e);
		/*
		if (FD_ISSET(i, &this->fdWrite))
			(this->*e->fds[i].clientWrite)(e);
		if (FD_ISSET(i, &this->fdRead) || FD_ISSET(i, &this->fdWrite))
			nFds--;
		*/i++;
	}
}

FileException::FileException( const char* msg ){
	this->msg = msg;
	if (std::strcmp(strerror(errno), "Success") != 0)
	{
		this->msg += " ";
		this->msg += strerror(errno);
	}
}
FileException::~FileException( void ) throw() {}
const char * FileException::what( void ) const throw() {
	return (msg.c_str());
}

