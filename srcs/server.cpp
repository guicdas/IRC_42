#include "../includes/irc.hpp"

Server::Server( void ){}

Server::Server( char **av ) : 
	password(av[2]),
	port(std::atoi(av[1])),
	clients(),
	channels()
{}

Server::Server( Server const &c ){
	*this = c;
}

Server	&Server::operator=( Server const &c ){
	if (this == &c)
		return (*this);
	return (*this);
}

Server::~Server( void ){}

void Server::acceptClient(void) {
    struct sockaddr_in clientAddr;
    socklen_t csin_len = sizeof(clientAddr);
    int clientSocket;

    memset(&clientAddr, 0, sizeof(clientAddr));

    clientSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddr, &csin_len);
    if (clientSocket < 0) {
        PRINT_COLOR(RED, "Failed to accept client!");
        return;
    }
	
    std::cout << "New client #" << clientSocket << " from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";

	if (clientSocket < 0)
		PRINT_COLOR(RED, "Invalid socket!");
    FD_SET(clientSocket, &this->fdList);
    this->maxFds = std::max(clientSocket, this->maxFds);

    this->clients.push_back(Client(clientSocket));
}


void	Server::createCommandMap( void ){
	this->commands["JOIN"] = &Server::join;
	this->commands["NICK"] = &Server::nick;
	this->commands["LIST"] = &Server::list;
	this->commands["MODE"] = &Server::mode;
	this->commands["QUIT"] = &Server::quit;
	this->commands["USER"] = &Server::user;
	this->commands["CAP"] = &Server::cap;
	this->commands["WHO"] = &Server::who;
	this->commands["PASS"] = &Server::pass;

	this->commands["TOPIC"] = &Server::topic;
	this->commands["INVITE"] = &Server::invite;
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
	if (bind(this->serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw (FileException("Error: bind(): "));
	if (listen(this->serverSocket, nRequests) == -1)
		throw (FileException("Error: listen(): "));
}

void Server::iterateClients(void) {
    for (std::vector<Client>::iterator it = this->clients.begin(); it != this->clients.end();)
	{
        Client &client = *it;
        if (FD_ISSET(client.getFd(), &this->fdRead))
		{
            if (clientRead(client) == 1) {
                std::cout << "client #" << client.getFd() << " gone, " 
                          << "Server has now " << this->clients.size() << " clients" << ENDL;
                it = this->clients.erase(it);
                continue;
            }
        }
        if (FD_ISSET(client.getFd(), &this->fdWrite)) {
            clientWrite(client);
        }
        ++it;
    }
}


void	Server::loop( void ){
	this->maxFds = this->serverSocket;
	int	nFds;

	FD_ZERO(&this->fdList);
	FD_ZERO(&this->fdWrite);
	FD_SET(this->serverSocket, &this->fdList);
	while (1)
	{
		this->fdWrite = this->fdRead = this->fdList;
		nFds = select(this->maxFds + 1, &this->fdRead, &this->fdWrite, NULL, NULL);
		if (nFds < 0)
			throw (FileException("Error: Select() negative return"));
		if (FD_ISSET(this->serverSocket, &this->fdRead))
			this->acceptClient();
		else 
			iterateClients();
	}
}
