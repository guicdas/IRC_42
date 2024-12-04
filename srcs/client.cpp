#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::parseCommand( t_client *client, std::string s )
{
	std::istringstream	input(s);
	std::string			arg;
	
	while (std::getline(input, arg, ' '))
		client->args.push_back(arg);
	
	std::map<std::string, void(Server::*)( t_client *, std::string )>::iterator itMap = this->commands.find(client->args.at(0));
	if (itMap != this->commands.end())
		(this->*itMap->second)(client, client->args.at(1));

}

int	Server::clientRead( t_client *c )
{
	char		buffer[BUF_SIZE] = {0};
	char		*commandLine;
	std::string	command;

	if (recv(c->fd, &buffer, BUF_SIZE, 0) < 1)
		return (0);

	commandLine = std::strtok(buffer, "\r\n");
	while (commandLine != NULL)
	{
		std::cout << "command to parse ->"<< commandLine << ENDL;
		command = commandLine;
		parseCommand(c, command);
		clientWrite(c);
		command.clear(); 						//necessario ?
		commandLine = std::strtok(NULL, "\r\n");
	}
	return (1);
}

void	Server::clientWrite( t_client *client )
{
	char		buffer[BUF_SIZE];

	std::strcpy(buffer, client->buffer.c_str());
	std::cout << "buf: ->" << buffer << ENDL;
	if (send(client->fd, &buffer, std::strlen(buffer), 0) < 1)
		std::cout << "client " << client->nickname << " DEUMERDA" << ENDL;
}

/*
/nick    gui  => (nick ja existe como channel) You are now known as gui
-NickServ- This nickname is registered. Please choose a different nickname, or identify via /msg NickServ IDENTIFY gui <password>

/join #gui,other => other => no such channel (gui ja existe)
/user  =>  You are already connected and cannot handshake again

(/list  =>  LIST :This command could not be completed because it has been used recently, and is rate-limited.)
*/
