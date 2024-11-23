#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::clientRead( t_client client )
{
	char		buffer[BUF_SIZE] = {0};
	std::size_t	found;
	int			received;

	received = recv(client.fd, &buffer, BUF_SIZE, 0);
	if (received < 1)
	{
		FD_CLR(client.fd, &this->fdRead);
		throw (FileException("client gone"));
	}
	client.buffer = buffer;
	std::cout << "message: " << client.buffer << "\n";
	client.buffer;
	
	std::map<std::string, void(*)()>::const_iterator it = this->commands.begin();
	found = client.buffer.find(10, 0);
	//if (found == std::string::npos)
	//	throw (FileException("fim"));
	//std::cout << "buffer " << client.buffer.c_str() << "." << std::endl << "token:" << client.buffer.substr(0, found);
	for (; it != this->commands.end(); it++)
	{
		if (std::strcmp(client.buffer.substr(0, found).c_str(), it->first.c_str()) == 0)
			it->second();
	}
}

void	Server::clientWrite( t_client client )
{
	(void)client;
}
