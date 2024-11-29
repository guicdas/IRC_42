#include "../includes/irc.hpp"

#define BUF_SIZE 4096

int	Server::clientRead( t_client *client )
{
	char		buffer[BUF_SIZE] = {0};

	if (recv(client->fd, &buffer, BUF_SIZE, 0) < 1)
		return (0);
	client->args = ircSplit(buffer, ' ');
	std:transform(client->args.at(0).begin(), client->args.at(0).end(), client->args.at(0).begin(), ::toupper);
	std::cout << "line received: " << buffer << std::endl;
	std::cout << "Command: " << client->args.at(0) << std::endl;
	
	client->buffer = "";

	std::map<std::string, void(Server::*)( t_client *, std::vector< std::string >)>::iterator itMap = this->commands.find(client->args.at(0));
	if (itMap != this->commands.end())
		(this->*itMap->second)(client, client->args);

	return (1);
}

void	Server::clientWrite( t_client *client )
{
	char		buffer[BUF_SIZE];

	//client->buffer = "@time " + client->name;
	std::strcpy(buffer, client->buffer.c_str());
	if (std::strlen(buffer) > 0)
	{
		if (send(client->fd, &buffer, std::strlen(buffer), 0) < 1)
			std::cout << "client " << client->name << " DEUMERDA";
	}
}

/*
/nick    gui  => (nick ja existe como channel) You are now known as gui
-NickServ- This nickname is registered. Please choose a different nickname, or identify via /msg NickServ IDENTIFY gui <password>

/join #gui,other => other => no such channel (gui ja existe)
/user  =>  You are already connected and cannot handshake again

(/list  =>  LIST :This command could not be completed because it has been used recently, and is rate-limited.)
*/
