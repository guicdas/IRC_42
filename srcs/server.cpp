#include "../includes/irc.hpp"

Server::Server( void ){}

Server::Server( char **av ){
	int	i = 0;

	this->password = av[2];
	this->port = std::atoi(av[1]);

	struct rlimit	rlp;
	if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
		throw (FileException("Error: getting rlimit!"));
	this->maxfd = rlp.rlim_cur;
	this->fds = (t_fd*) malloc(sizeof(*this->fds) * this->maxfd);
	if (this->fds == NULL)
		throw (FileException("Error: could not allocate fds!"));

	while (i < this->maxfd)
	{
		this->fds[i].type = 0;
		this->fds[i].fct_read = NULL;
		this->fds[i].fct_write = NULL;
		i++;
	}
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

static void	client_read( void )//t_env *e, int cs)
{

}

static void	client_write( void )//t_env *e, int cs)
{

}

static void	acceptClient( void ){
	int	i = 0;
	/* extract the first connection on the queue of pending connections, create a 
	new socket with the same socket type protocol and address family as the specified 
	socket, and allocate a new file descriptor for that socket */
	this->clientSocket = accept(this->serverSocket, (struct sockaddr*)&this->clientAddr, (socklen_t *)sizeof(clientAddr));
	std::cout << "New client #" << this->clientSocket << " from " 
	<< inet_ntoa(this->clientAddr.sin_addr) << ":" << ntohs(this->clientAddr.sin_port) << "\n";

	while (i < this->maxfd)
	{
		this->fds[i].type = 0;
		this->fds[i].fct_read = NULL;
		this->fds[i].fct_write = NULL;
		i++;
	}
	this->fds[this->clientSocket].type = 2;
	this->fds[this->clientSocket].fct_read = client_read;
	this->fds[this->clientSocket].fct_write = client_write;
}

void	Server::createServerSocket( void ){
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
	/* The bind() function binds a unique local name to the socket with descriptor socket. */
	if (bind(this->serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw (FileException("Error: bind(): "));
	/* transforms an active socket into a passive socket
	Once called, socket can never be used as an active socket to initiate connection requests 
	It indicates a readiness to accept client connection requests, and creates a connection 
	request queue of length backlog to queue incoming connection requests. 
	Once full, additional connection requests are rejected.*/
	if (listen(this->serverSocket, nRequests) == -1)
		throw (FileException("Error: listen(): "));
	this->fds[this->serverSocket].type = 1;
	this->fds[this->serverSocket].fct_read = acceptClient;
}


void	Server::loop( void ){
	int	i = 0;
	int	maxFds = 0;
	int	nFds;

	FD_ZERO(&this->fd_read);
	FD_ZERO(&this->fd_write);
	while (i < this->maxfd)
	{
		if (this->fds[i].type != 0)
		{
			FD_SET(i, &this->fd_read);
			if (strlen(this->fds[i].buf_write) > 0)
				FD_SET(i, &this->fd_write);
			maxFds = (maxFds > i) ? maxFds : i;
		}
		i++;
	}
	nFds = select(maxFds + 1, &this->fd_read, &this->fd_write, NULL, NULL);
	i = 0;
	while ((i < this->maxfd) && (nFds > 0))
	{
		if (FD_ISSET(i, &this->fd_read))
			this->fds[i].fct_read();
		if (FD_ISSET(i, &this->fd_write))
			this->fds[i].fct_write();
		if (FD_ISSET(i, &this->fd_read) || FD_ISSET(i, &this->fd_write))
			nFds--;
		i++;
	}
}

bool Server::signal = false;
void Server::signalHandler( int signum )
{
	std::cout << std::endl << "Signal Received: " << signum << std::endl;
	Server::signal = true; //-> set the static boolean to true to stop the server
}

FileException::FileException( const char* msg ){
	this->msg = msg;
	this->msg += strerror(errno);
}
FileException::~FileException( void ) throw() {}
const char * FileException::what( void ) const throw() {
	return (msg.c_str());
}

