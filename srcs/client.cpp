#include "../includes/irc.hpp"

#define BUF_SIZE 4096

void	Server::parseCommand( t_client &client, std::string s )
{
	std::istringstream	input(s);
	std::string			arg;
	
	while (std::getline(input, arg, ' '))
		client.args.push_back(arg);

	for (int i = 0; client.args.at(0)[i] ;i++)
	{
		if (client.args.at(0)[i] >= 'a' && client.args.at(0)[i] <= 'z')
			client.args.at(0)[i] = toupper(client.args.at(0)[i]);
	}	
	
	std::map<std::string, void(Server::*)( t_client & )>::iterator itMap = this->commands.find(client.args.at(0));
	if (itMap != this->commands.end())
		(this->*itMap->second)(client);
	else
		Server::privmsg(client);
}

int	Server::clientRead( t_client &c )
{
	char		buffer[BUF_SIZE] = {0};
	char		*commandLine;
	std::string	command;
	int			bytesRecv;

	bytesRecv = recv(c.fd, &buffer, BUF_SIZE, 0);
	if (bytesRecv < 1)
	{
		if (bytesRecv < 0)
			std::cerr << "recv error" << std::endl;
		return (0);	
	}

	commandLine = std::strtok(buffer, "\r\n");
	while (commandLine != NULL)
	{
		std::cout << "\ncmd >"<< commandLine << ENDL;
		command = commandLine;
		parseCommand(c, command);
		c.buffer.clear();
		c.args.clear();
		commandLine = std::strtok(NULL, "\r\n");
	}
	c.buffer.clear();
	c.args.clear();
	return (1);
}

void	clientWrite( t_client &client )
{
	char	buffer[BUF_SIZE];
	int		bytesSent;

	std::strcpy(buffer, client.buffer.c_str());

	bytesSent = send(client.fd, buffer, std::strlen(buffer), 0);
	if (std::strlen(buffer) > 0)
		std::cout << "sent >>" << buffer << ENDL;
	client.buffer.clear();
}
