#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::clientRead( t_client *client )
{
	char		buffer[BUF_SIZE] = {0};
	std::size_t	found;

	//std::cout << "client infos:\n\t- name: " << client->name << \
	"\n\t- buffer: " << client->buffer << "\n\t- fd: " << client->fd << std::endl;
	// nome n fica guardado

	if (recv(client->fd, &buffer, BUF_SIZE, 0) < 1)
	{
		FD_CLR(client->fd, &this->fdRead);
		throw (FileException("client gone"));
	}
	client->buffer = buffer; //while 

	found = client->buffer.find(' ', 0);
	if (found == std::string::npos)
		found = client->buffer.find(10, 0);
	std::map<std::string, void(Server::*)( t_client *, std::string string )>::const_iterator it = this->commands.find(client->buffer.substr(0, found));
	
	if (it != this->commands.end())
	{
		std::cout << "encontrou\t"<<  it->first << std::endl;
		(this->*it->second)(client, client->buffer);
	}
	//std::cout << client.buffer << std::endl;
}

void	Server::clientWrite( t_client *client )
{
	char		buffer[BUF_SIZE];

	std::cout << "client infos:\n\t- name: " << client->name << \
	"\n\t- buffer: " << client->buffer << "\n\t- fd: " << client->fd << std::endl;
	// nada fica guardado

	std::strcpy(buffer, client->buffer.c_str());
	if (send(client->fd, &buffer, std::strlen(buffer), 0) < 1)
		throw (FileException("deu merda"));
}

/*
/nick    gui  => (nick ja existe como channel) You are now known as gui
-NickServ- This nickname is registered. Please choose a different nickname, or identify via /msg NickServ IDENTIFY gui <password>

/join #gui,other => other => no such channel (gui ja existe)
/user  =>  You are already connected and cannot handshake again

(/list  =>  LIST :This command could not be completed because it has been used recently, and is rate-limited.)
*/
