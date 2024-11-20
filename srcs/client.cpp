#include "../includes/irc.hpp"

void	Server::clientRead( t_client client )
{
	int	received, i = 0;

	received = recv(client.fd, &client.buffer, 4096, 0);
	if (received < 1)
	{
		close(client.fd);
		throw (FileException("client gone"));
	}
	/*
	else
	{
		while (i < e->maxFd)
		{
			if ((e->fds[i].type == 2) && (i != this->clientSocket))
				send(i, e->fds[this->clientSocket].readBuf, received, 0);
			i++;
		}
	}
	*/
}

void	Server::clientWrite( t_client client )
{
	(void)client;
}
