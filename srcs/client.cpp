#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::clientRead( t_client client )
{
	int	received;
	char buffer[BUF_SIZE] = {0};

	received = recv(client.fd, &buffer, BUF_SIZE, 0);
	if (received < 1)
	{
		FD_CLR(client.fd, &this->fdRead);
		throw (FileException("client gone"));
	}
	client.buffer = buffer;
	std::cout << client.buffer << "\n";
	
}

void	Server::clientWrite( t_client client )
{
	(void)client;
}
