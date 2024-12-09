#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::parseCommand( t_client *client, std::string s )
{
	std::istringstream	input(s);
	std::string			arg;
	
	while (std::getline(input, arg, ' '))
		client->args.push_back(arg);
	
	std::map<std::string, void(Server::*)( t_client *, std::vector< std::string > )>::iterator itMap = this->commands.find(client->args.at(0));
	if (itMap != this->commands.end())
	{
		(this->*itMap->second)(client, client->args);
	}
	else
		Server::privmsg(client, client->args);
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
		std::cout << "\ncommand >"<< commandLine << ENDL;
		command = commandLine;
		parseCommand(c, command);
		clientWrite(c);
		command.clear(); 						//necessario ?
		commandLine = std::strtok(NULL, "\r\n");
	}
	return (1);
}

void	clientWrite( t_client *client )
{
	char		buffer[BUF_SIZE];

	std::strcpy(buffer, client->buffer.c_str());
	std::cout << ">>" << buffer << ENDL;
	if (send(client->fd, &buffer, std::strlen(buffer), 0) < 1)
		std::cout << client->nickname << " (buffer vazio, n mandou)" << ENDL;
}
