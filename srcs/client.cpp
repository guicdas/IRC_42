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
	//std::cout << "message: " << client.buffer << std::endl;
	
	std::map<std::string, void(Server::*)( t_client, std::string string )>::iterator it = this->commands.begin();
	found = client.buffer.find(' ', 0);
	if (found == std::string::npos)
	{
		found = client.buffer.find(10, 0);
	}
	else
	{
		for (; it != this->commands.end(); it++)
		{
			if (std::strcmp(client.buffer.substr(0, found).c_str(), it->first.c_str()) == 0)
				(this->*it->second)(client, client.buffer.substr(++found, client.buffer.size()));		// /nick com dois espacos da merda => tira los 
		}
		// comand not found
	}
}

/*
* /nick    gui  => (nick ja existe como channel) You are now known as gui
-NickServ- This nickname is registered. Please choose a different nickname, or identify via /msg NickServ IDENTIFY gui <password>


/join #gui,other => other => no such channel (gui ja existe)
/user  =>  You are already connected and cannot handshake again

(/list  =>  LIST :This command could not be completed because it has been used recently, and is rate-limited.)
*/
void	Server::clientWrite( t_client client )
{
	(void)client;
}
