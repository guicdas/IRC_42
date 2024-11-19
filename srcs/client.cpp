#include "../includes/irc.hpp"

void	Server::clientRead( t_env *e )
{
	int	received, i = 0;

	received = recv(this->clientSocket, e->fds[this->clientSocket].readBuf, BUF_SIZE, 0);
	if (received < 1)
	{
		close(this->clientSocket);
		while (i < e->maxFd)
		{
			clean_fd(&e->fds[i]);
			i++;
		}
		throw (FileException("client gone"));
	}
	else
	{
		while (i < e->maxFd)
		{
			if ((e->fds[i].type == 2) && (i != this->clientSocket))
				send(i, e->fds[this->clientSocket].readBuf, received, 0);
			i++;
		}
	}
}

void	Server::clientWrite( t_env *e )
{
	(void)e;
}
